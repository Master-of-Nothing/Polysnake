#include "Game/Graphics/GraphObjects/Fruit.h"
#include <NucleoImp/Display/ILI9341Display.h>

#define TILE_SIZE 10

namespace ELE3312 {

Fruit::Fruit(){};

Fruit::Fruit(ILI9341Display* display)
{
	this -> disp = display;
    for (int i = 0; i <= LISTE_FRUITS; i++)
    {
    	fruits[i].x = (rand() % 31) * 10 + i * TILE_SIZE;
    	fruits[i].y = ((rand() % 23)) * 10;
    	fruits[i].id = 3 + (rand() % 4);
    }
}

void Fruit::draw()
{
	for (int i = 0; i <= LISTE_FRUITS; i++)
	{
		Color color;
		switch (fruits[i].id)
			{
			case 3 : color = Color::RED ; break;
			case 4 : color = Color::BLUE; break;
			case 5 : color = Color::ORANGE; break;
			case 6 : color = Color::YELLOW; break;
			}
		disp->fillRect(color, fruits[i].x, fruits[i].y, TILE_SIZE, TILE_SIZE);
	}
}

tile* Fruit::getFruit(){ return this->fruits;}

void Fruit::clear(int xPos, int yPos)
{
	for(int i = 0; i < LISTE_FRUITS; i++)
	{
		if(fruits[i].x == xPos && fruits[i].y == yPos)
		{
			fruits[i].x = 0;
			fruits[i].y = 0;
			fruits[i].id = empty;
		}
	}
}

bool Fruit::find(int xPos, int yPos)
{
	for(int i = 0; i < LISTE_FRUITS; i++)
		{
			if(fruits[i].x == xPos && fruits[i].y == yPos)
			{
				return 1;
			}
		}
	return 0;
}

}
