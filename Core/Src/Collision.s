	.syntax unified
	.thumb
	.text
	.align

	.global FruitCollision_asm
	.type FruitCollision_asm, %function
	.global SnakeCollision_asm
	.type SnakeCollision_asm, %function


// r0 : tile* liste_fruit
// r1 : tile* head
// r2 : nombre_fruit
FruitCollision_asm:
    PUSH {r4-r7, lr}

    // On charge la tête du serpent UNE SEULE FOIS
    LDR r4, [r1]      // r4 = head.x
    LDR r5, [r1, #4]  // r5 = head.y

    MOV r3, #0        // i = 0 (compteur)

loop_fruit:
    CMP r3, r2        // i == nombre_fruit ?
    BGE no_collision

    // Calcul de l'adresse du fruit actuel (i * 12)
    MOV r6, #12
    MUL r6, r3, r6

    // Test X
    LDR r7, [r0, r6]  // r7 = fruit[i].x
    CMP r7, r4
    BNE next_fruit

    // Test Y
    ADD r6, r6, #4    // Offset pour Y
    LDR r7, [r0, r6]  // r7 = fruit[i].y
    CMP r7, r5
    BEQ collision_found // Trouvé !

next_fruit:
    ADD r3, r3, #1
    B loop_fruit

collision_found:
    MOV r0, #1        // Renvoie True
    POP {r4-r7, pc}

no_collision:
    MOV r0, #0        // Renvoie False
    POP {r4-r7, pc}
    BX lr



// r0 : adresse base du tableau (tile*)
// r1 : index actuel de la tête (0-99)
// r2 : longueur actuelle du serpent (nombre d'anneaux)
// r3 : non utilisé en entrée (on fixe 100 dans le code)

SnakeCollision_asm:
    PUSH {r4-r8, lr}

    // 1. Charger les coordonnées de la TÊTE
    MOV r4, #12
    MUL r4, r1, r4      // Offset tête = index * 12
    LDR r5, [r0, r4]    // r5 = head.x (offset 0)
    ADD r4, r4, #4
    LDR r6, [r0, r4]    // r6 = head.y (offset 4)

    // 2. Initialiser la boucle sur le CORPS
    // On commence à l'anneau précédent la tête
    MOV r7, r1          // r7 = index courant
    SUB r2, r2, #1      // On ne teste pas la tête (longueur - 1)

loop_circular:
    CMP r2, #0          // Plus d'anneaux à tester ?
    BEQ no_collision

    // Reculer l'index (on remonte de la tête vers la queue)
    SUBS r7, r7, #1
    BMI reset_index     // Si r7 < 0, on reboucle à 99
    B calc_addr

reset_index:
    MOV r7, #99         // Taille max - 1

calc_addr:
    MOV r4, #12
    MUL r4, r7, r4      // Offset de l'anneau courant
    ADD r8, r0, r4      // Adresse de l'anneau courant (base + offset)

    // 3. Comparaisons
    LDR r4, [r8]        // r4 = body[i].x
    CMP r4, r5
    BNE next_iter       // X différent ? On passe à la suite

    LDR r4, [r8, #4]    // r4 = body[i].y
    CMP r4, r6
    BEQ collision_found // X et Y identiques ? Collision !

next_iter:
    SUB r2, r2, #1
    B loop_circular

collision_found:
    MOV r0, #1
    B end_asm

no_collision:
    MOV r0, #0

end_asm:
    POP {r4-r8, pc}
    BX lr

