
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

using namespace ELE3312;

ILI9341Display display;

struct tile
{
	int x;
	int y;
	int id;
};

void drawSnake()
{
	display.fillRect(Color::DARKGREEN, 50, 50, 10, 10);
	display.fillRect(Color::GREEN, 40, 50, 10, 10);
}

int rowID()
{
	return ((rand() % 31)*10);
}
int columnID()
{
	return ((rand() % 23)*10);
}

int colorID()
{
	return (rand() % 4);
}

Color getFruitColor()
{
	int color = colorID() ;
		switch (color)
		{
		case 0 : return Color::YELLOW;
		case 1 : return Color::ORANGE;
		case 2 : return Color::PURPLE;
		case 3 : return Color::RED;
		}
}

void drawFruits()
{
	int rowIDs[10], columnIDs[10], x, y;
	Color colors[5], color;
	for (int i = 0; i < 10; i++)
	{
		rowIDs[i] = rowID();
		columnIDs[i] = columnID();
		colors[i] = getFruitColor();
		display.fillRect(colors[i], rowIDs[i], columnIDs[i], 10, 10);
	}
}

void cpp_main(peripheral_handles *handles)
{
	display.setup(handles->hspi_tft);
	display.clearScreen();
	drawFruits();
	drawSnake();
	while(1){};
}
