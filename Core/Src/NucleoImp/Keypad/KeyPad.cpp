/*
 * Keypad.c
 *
 *  Created on: Mar 16, 2026
 *      Author: thom
 */

#include <NucleoImp/Keypad/KeyPad.h>

using namespace ELE3312;

KeyPad::KeyPad(){}

void KeyPad::setup(GPIO_TypeDef * gpio){
	this->gpio = gpio;
	update();
}

KeyCode KeyPad::update(){
	keypressed = 0; // remets état du clavier à aucune touche préssée
	for(uint32_t row = NumRow; row > 0 ; row--)
			{

				// Première chose : mettre à jour ODR pour la colonne préssée
				// 0x0138 : mette toutes les pins des rangées à HIGH, équivalent à ROW1_PIN |ROW2_PIN |ROW3_PIN |ROW4_PIN |
				constexpr uint32_t high = ROW1_Pin | ROW2_Pin | ROW3_Pin | ROW4_Pin;
				gpio->ODR |= high;			// 0x0170;
				// On mets la pin de row à low pour selectinner la ligne
				switch (row)
				{
					case 1:
						// ROW1_PIN = 0x0010
						gpio->ODR &= ~0x10;	//ROW1_Pin;
						//chiffre = 1;
						break;
					case 2:
						// ROW2_PIN = 0x0020
						gpio->ODR &= ~0x20;	//ROW2_Pin;
						//chiffre = 2;
						break;
					case 3:
						// ROW3_PIN = 0x0040
						gpio->ODR &= ~0x40;	//ROW3_Pin;
						//chiffre = 3;
						break;
					case 4:
						// ROW4_PIN = 0x0100
						gpio->ODR &= ~0x100; //ROW4_Pin;
						//chiffre = 4;
						break;
				}
				HAL_Delay(10);

				// Deuxième chose : on regarde si une touche est présée
				//On shift les 4 derniers bits de keypressed pour s'assurer que les bits soit à 0
				uint32_t keysPressed = 0;// = 0
				keysPressed = keysPressed << 4;
				// IDR : dans les quatres derniers bits, celui qui passe à zero est celui ou une touche de la colonne à été pressé
				// 0xF : masque definnissant le nombre de colonne
				// keysPressed : bit 1 indique la position de la colonne pressée
				keysPressed |= (~gpio->IDR) & 0xF;
				HAL_Delay(10);


				uint8_t colum = 0;
				if( keysPressed == 1){
					colum = 0;
					keypressed = 1;}
				else if( keysPressed == 2){
					colum = 1;
					keypressed = 1;}
				else if( keysPressed == 4){
					colum = 2;
					keypressed = 1;}
				else if( keysPressed == 4){
					colum = 3;
					keypressed = 1;}
				if(keypressed) return key = keyboard[row - 1][colum];

				/*if (row == 2 && keysPressed == 4) {
					keypressed = 1;
					return key = KeyCode::SIX;
					}
				else if (row == 2 && keysPressed == 1) {
					keypressed = 1;
					return key = KeyCode::FOUR;
					}*/
			}
	return  key = KeyCode::UNKNOWN;
}

KeyCode KeyPad::keyPress(){
	if(!keypressed) // aucune touche a été nouvellement préssée
		return KeyCode::UNKNOWN;
	return key;
}








