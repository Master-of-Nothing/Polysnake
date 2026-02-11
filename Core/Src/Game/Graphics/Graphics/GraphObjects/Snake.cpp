#include "Game/Graphics/GraphObjects/Snake.h"
#include <NucleoImp/Display/ILI9341Display.h>

#define TILE_SIZE 10

namespace ELE3312 {


Snake::Snake(ILI9341Display* display)
{
    this -> disp = display;
    this -> head = 3;
    this -> tail = 0;
    this -> LastDirection = EAST;

    int xInit = (rand() % 31) * 10;
    int yInit = (rand() % 23) * 10;

    for (int i = 0; i <= head; i++)
    {
    	tampon[i].x = xInit + (i * TILE_SIZE);
    	tampon[i].y = yInit;
    	tampon[i].id = (i == head) ? snakehead : snakebody;
    }
}

void Snake::draw()
{
	for (int i = 0; i <= (head + tail); i++)
	{
		Color color = (i == head) ? Color::DARKGREEN : Color::GREEN ;
		disp->fillRect(color, tampon[i].x, tampon[i].y, TILE_SIZE, TILE_SIZE);
	}
}

void Snake::clear()
{
    disp->fillRect(Color::BLACK, tampon[tail].x, tampon[tail].y, TILE_SIZE, TILE_SIZE);
}

void Snake::move(int eat)
{
	int headInitial = Snake::getHead();

	if(!eat)
	{
		tampon[tail].id = empty; // 0 case vide, la queue a avancer
		tail++; // Attention verifier pour le ring buffer
	}

	tampon[head].id = snakebody; // la case de la tete devient un corps
	switch (LastDirection)
		{
		case(NORTH):
				;
		case(EAST):
				;
		case(SOUTH):
				;
		case(WEST):
				;
		}
}

void Snake::turn(int direction)
{
	if(direction == NORTH || direction == EAST || direction == SOUTH || direction == WEST)
		Snake::LastDirection = direction;
}

int Snake::getHead() { return this -> head; }

int Snake::getTail() { return this -> tail; }

tile* Snake::getTampon() { return this->tampon; }

int Snake::getLastDirection() { return Snake::LastDirection; }

}
