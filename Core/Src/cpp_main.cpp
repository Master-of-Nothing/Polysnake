
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
#include "Interfaces/Keypad/Keypad.h"
#include <Game/Graphics/GraphObjects/Snake.h>
#include <Game/Graphics/GraphObjects/Fruit.h>
#include "Game/Game.h"

using namespace ELE3312;

ILI9341Display display;
GPIOKeypad keypad;


//struct tile
//{
//	int x;
//	int y;
//	int id;
//};
//
//
//enum tileType
//{
//	Vide = 0,
//	SnakeHead = 1,
//	SnakeBody = 2,
//	FruitYellow = 3,
//	FruitRed = 4,
//	FruitPurple = 5,
//	FruitOrange = 6
//};
//
//Color colorType(int id) {
//	switch (id)
//	{
//	case 0 : return Color::BLACK;
//	case 1 : return Color::DARKGREEN;
//	case 2 : return Color::GREEN;
//	case 3 : return Color::YELLOW;
//	case 4 : return Color::RED;
//	case 5 : return Color::PURPLE;
//	case 6 : return Color::ORANGE;
//	}
//}
//
//void drawTile(tile t)
//{
//	display.fillRect(colorType(t.id), t.x * 10, t.y * 10, 10, 10);
//}
//
//void drawSnake()
//{
//	drawTile({3, 5, SnakeBody});
//	drawTile({4, 5, SnakeBody});
//	drawTile({5, 5, SnakeHead});
//}
//
//void drawFruits()
//{
//	for (int i = 0; i < 12; i++)
//	{
//		int rowID = rand() % 23;
//		int columnID = rand() % 31;
//		int fruitID = 3 + (rand() % 4);
//		drawTile({rowID, columnID, fruitID});
//	}
//}
void cpp_main(peripheral_handles *handles)
{

	display.setup(handles->hspi_tft);
	display.clearScreen();
	//keypad.setup(handles->gpio_keypad);
	GPIO_TypeDef * gpio = handles->gpio_keypad;

	Fruit fruit(&display);
	fruit.draw();
	Snake snake(&display);
	snake.draw();
	uint32_t row = 4;
	uint32_t keysPressed = 0;
	int chiffre = 0;
	while(1)
	{
		for(int i = 0; i < 4; i++)
		{
			// Première chose : mettre à jour ODR pour la colonne préssée
					// 0x0138 : mette toutes les pins des rangées à HIGH, équivalent à ROW1_PIN |ROW2_PIN |ROW3_PIN |ROW4_PIN |
					if(row == 0)
						row = 4;
					else
						row--;
					constexpr uint32_t high = ROW1_Pin | ROW2_Pin | ROW3_Pin | ROW4_Pin;
					gpio->ODR |= high;// 0x0170;
					// On mets la pin de row à low pour selectinner la ligne
					switch(row)
					{
					case 1 :
						// ROW1_PIN = 0x0010
						gpio->ODR &= ~0x10;//ROW1_Pin;
						break;
					case 2 :
						// ROW2_PIN = 0x0020
						gpio->ODR &= ~0x20;//ROW2_Pin;
						break;
					case 3 :
						// ROW3_PIN = 0x0040
						gpio->ODR &= ~0x40;//ROW3_Pin;
						break;
					case 4 :
						// ROW4_PIN = 0x0100
						gpio->ODR &= ~0x100;//ROW4_Pin;
						break;
					}
					// Ajoute un delai pour s'assurer que IDR puisse se mettre á jour
					HAL_Delay(50);
				// Deuxième chose : on regarde si une touche est présée
					//On shift les 4 derniers bits de keypressed pour s'assurer que les bits soit à 0
					keysPressed = 0;
					keysPressed = keysPressed << 4 ;
					// IDR : dans les quatres derniers bits, celui qui passe à zero est celui ou une touche de la colonne à été pressé
					// 0xF : masque definnissant le nombre de colonne
					// keysPressed : bit 1 indique la position de la colonne pressée
					keysPressed |= (~gpio->IDR) & 0xF;
				// On regarde quelle touche à été préssée
					if(row == 1 && keysPressed == 2)
					{
						// change la direction pour le NORD
						snake.turn(1); // NORTH

					}
					else if(row == 2 && keysPressed == 4)
					{
						// change la direction pour l'OUEST
						snake.turn(2); // EAST
					}
					else if(row == 3 && keysPressed == 2)
					{
						// change la direction pour l'OUEST
						snake.turn(3); // SOUTH
					}
					else if(row == 2 && keysPressed == 1)
					{
						// change la direction pour l'OUEST
						snake.turn(4); // WEST
					}
		}

		snake.move(0);
		snake.draw();
		//HAL_Delay(200);
	};
}
