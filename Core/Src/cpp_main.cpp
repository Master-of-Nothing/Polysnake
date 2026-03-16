
// * @file cpp_main.cpp
//  * @date June 2025
//  * @brief
//  *
//  * @defgroup ELE3312
//  * @{


#include <memory>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <ctime>

#include "main.h"
#include <cpp_main.h>
#include <NucleoImp/Display/ILI9341Display.h>
#include <NucleoImp/MotionInput/MPU6050MotionInput.h>
#include <Game/Graphics/GraphObjects/Snake.h>
#include <Game/Graphics/GraphObjects/Fruit.h>
#include "Game/Game.h"

#define TILE_SIZE 10

using namespace ELE3312;

ILI9341Display display;
//GPIOKeypad keypad;
MPU6050MotionInput motionInput;

extern "C"{
	bool FruitCollision_asm(ELE3312::tile* liste_fruit, ELE3312::tile* snake_head, int nombre_fruit);
}
extern "C"{
	bool SnakeCollision_asm(ELE3312::tile* snake_body,int head_index, int longueur_snake);
}




// Fonction permettant de détecter si le serpent va manger un fruit.
// Sera prochainement implémenté dans Game.cpp
int eat(Snake snake, Fruit fruit)
{
	int LastDirection = snake.getLastDirection();
	int head = snake.getHead();
	tile* tampon = snake.getTampon();
	int eat = 0;
	if(LastDirection == NORTH)
	{
		eat = fruit.find(tampon[head].x,tampon[head].y - TILE_SIZE);
		if(!eat){fruit.clear(tampon[head].x,tampon[head].y - TILE_SIZE);}
		return eat;
	}
	else if (LastDirection == EAST)
	{
		eat = fruit.find(tampon[head].x + TILE_SIZE,tampon[head].y);
		if(!eat){fruit.clear(tampon[head].x + TILE_SIZE,tampon[head].y);}
		return eat;
	}
	else if (LastDirection == SOUTH)
	{
		eat = fruit.find(tampon[head].x,tampon[head].y + TILE_SIZE);
		if(!eat){fruit.clear(tampon[head].x,tampon[head].y + TILE_SIZE);}
		return eat;

	}
	else if (LastDirection == WEST)
	{
		eat = fruit.find(tampon[head].x - TILE_SIZE,tampon[head].y);
		if(!eat){fruit.clear(tampon[head].x - TILE_SIZE,tampon[head].y);}
		return eat;
	}
	return eat;
}
Game game;
void cpp_main(peripheral_handles *handles)
{
	display.setup(handles->hspi_tft);
	display.clearScreen();
	GPIO_TypeDef *gpio = handles->gpio_keypad;
	//keypad.setup(handles->gpio_keypad);
	motionInput.setup(handles->hi2c);
	Fruit fruit(&display);
	fruit.draw();
	Snake snake(&display);
	snake.draw();
	uint32_t keysPressed = 0;
	bool state = 1;
	while(1)
	{
		if(!state) // commence la partie => initaialisation
		{
			display.clearScreen();
			HAL_Delay(2);
			//keypad.setup(handles->gpio_keypad);
			Fruit fruit(&display);
			fruit.draw();
			Snake snake(&display);
			snake.draw();
			state = 1;
		}
		// A retrvailler ecran a des reaction bizarre
//		keypad.update();
//		if(keypad.isAnyKeyPressed())
//		{
//			switch(keypad.getFirstKeyPressed())
//			{
//			case KeyCode::FOUR:
//				snake.turn(4);
//				break;
//			case KeyCode::SIX:
//				snake.turn(2);
//				break;
//			default:
//				break;
//			}
//		}
		for(uint32_t row = 4; row > 0 ; row--)
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
					HAL_Delay(10);
				case 2:
					// ROW2_PIN = 0x0020
					gpio->ODR &= ~0x20;	//ROW2_Pin;
					//chiffre = 2;
					HAL_Delay(10);
					//break;
				case 3:
					// ROW3_PIN = 0x0040
					gpio->ODR &= ~0x40;	//ROW3_Pin;
					//chiffre = 3;
					HAL_Delay(10);
				case 4:
					// ROW4_PIN = 0x0100
					gpio->ODR &= ~0x100; //ROW4_Pin;
					//chiffre = 4;
					HAL_Delay(10);
			}

			// Deuxième chose : on regarde si une touche est présée
			//On shift les 4 derniers bits de keypressed pour s'assurer que les bits soit à 0
			keysPressed = 0;
			keysPressed = keysPressed << 4;
			// IDR : dans les quatres derniers bits, celui qui passe à zero est celui ou une touche de la colonne à été pressé
			// 0xF : masque definnissant le nombre de colonne
			// keysPressed : bit 1 indique la position de la colonne pressée
			keysPressed |= (~gpio->IDR) & 0xF;
			HAL_Delay(30);
			// On regarde quelle touche à été préssée
			if (row == 1 && keysPressed == 2) {
				// change la direction pour le NORD
				snake.turn(NORTH); // NORTH

			} else if (row == 2 && keysPressed == 4) {
				// change la direction pour l'OUEST
				snake.turn(EAST); // EAST
			} else if (row == 3 && keysPressed == 2) {
				// change la direction pour l'OUEST
				snake.turn(SOUTH); // SOUTH
			} else if (row == 2 && keysPressed == 1) {
				// change la direction pour l'OUEST
				snake.turn(WEST); // WEST
			}
		}
		if(!keysPressed)
		{
			motionInput.update();
			if(motionInput.getX() > 0.6 && motionInput.getX() > 0.3)
				snake.turn(WEST);
			else if(motionInput.getY() > 0.6 && motionInput.getY() > 0.3)
				snake.turn(EAST);
			HAL_Delay(5);
		}
		tile* liste_fruit = fruit.getFruit();
		tile* snake_body = snake.getTampon();
		if(SnakeCollision_asm(snake_body,snake.getHead(), snake.getLongueur()))
		{
			state = 0;
			display.clearScreen();
			continue;
		}
		//FruitCollision_asm(liste_fruit, snake_body + snake.getHead(), 10)
		snake.move(FruitCollision_asm(liste_fruit, snake_body + snake.getHead(), 10));
		snake.draw();
	};
}
