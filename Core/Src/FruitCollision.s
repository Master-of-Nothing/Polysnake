	.syntax unified
	.thumb
	.text
	.align 2

	.global FruitCollision_asm
	.global SnakeCollision_asm



FruitCollision :
// On suppose que les positions ont déjà été mise à jour suite au changement de direction.
// Pas encore affiché graphiquement
// bool FruitCollision(tile* liste_fruit, tile* tampon[head], int nombre_fruit);
// r0 : liste_fruit
// r1 : tampon[head]
// r2 : nombre_fruit sur 32 bit
	PUSH{lr}
	;MOV r3, #10; i = 1

loop CMP r3, #0
	 BL END ; sortie de boucle
	 CMP  ; verifier si head_serpent.x == liste_fruit[].x

END POP{lr}
