
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
#include <Game/Graphics/GraphObjects/Snake.h>
#include <Game/Graphics/GraphObjects/Fruit.h>

using namespace ELE3312;

ILI9341Display display;

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
	//drawFruits();
	//drawSnake();
	Snake snake(&display);
	snake.draw();
	Fruit fruit(&display);
	fruit.draw();
	while(1)
	{};
}
