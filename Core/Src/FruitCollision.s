	.syntax unified
	.thumb
	.text
	.align 2

	.global FruitCollision_asm
	.global SnakeCollision_asm



FruitCollision_asm :
// On suppose que les positions ont déjà été mise à jour suite au changement de direction.
// Pas encore affiché graphiquement
// bool FruitCollision(tile* liste_fruit, tile* tampon[head], int nombre_fruit);
// r0 : liste_fruit
// r1 : tampon[head]
// r2 : nombre_fruit sur 32 bit
	PUSH{lr, r4, r5, r6}
	MOV r3, #0 ; i = 1
	MOV r5, #0 ; deplacement dans la matrice

loop CMP r3, r2
	 BEQ END ; sortie de boucle
	 UMULL r5, r3, #12 ;
	 LDR r4, [r0, r5]  ; fruit[].x
	 LDR r6 , [r1] ; snake.head[].x
	 CMP r4, r6 ; snake.head[].x == fruit[].x
	 BNE continue
	 ADD r5, #4
	 LDR r2, [r0, r5]
	 LDR r6, [r1, #4]
	 CMP r4, r6 ; snake.head[].y == fruit[].y
	 BNE continue
	 MOV r0, #1
	 END POP{lr, r4, r5, r6}
	 BX lr

continue ADD r3, #1
		 B loop

SnakeCollision_asm :
// On suppose que les positions ont déjà été mise à jour suite au changement de direction.
// Pas encore affiché graphiquement
// bool FruitCollision(tile* liste_fruit, tile* tampon[head], int nombre_fruit);
// r0 : tampon[]
// r1 : tampon[head]
// r2 : longeur snake
	 PUSH{lr, r4, r5, r6}
	 MOV r3, #0 ; i = 1
	 MOV r5, #0 ; deplacement dans la matrice

loop CMP r3, r2
	 BEQ END ; sortie de boucle
	 UMULL r5, r3, #12 ;
	 LDR r4, [r0, r5]  ; snake[].x
	 LDR r6 , [r1] ; snake.head[].x
	 CMP r4, r6 ; snake.head[].x == snake[].x
	 BNE continue
	 ADD r5, #4
	 LDR r2, [r0, r5]
	 LDR r6, [r1, #4]
	 CMP r4, r6 ; snake.head[].y == snake[].y
	 BNE continue
	 MOV r0, #1
	 END POP{lr, r4, r5, r6}
	 BX lr

continue ADD r3, #1
		 B loop
