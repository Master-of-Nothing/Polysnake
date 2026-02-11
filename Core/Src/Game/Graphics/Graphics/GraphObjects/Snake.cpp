#include "Game/Graphics/GraphObjects/Snake.h"
#include <NucleoImp/Display/ILI9341Display.h>

namespace ELE3312 {


Snake::Snake(ILI9341Display* display)
{
    this->disp = display;
    LastDirection = East;
}

void Snake::draw()
{

	disp->fillRect(Color::RED, 50, 50, 10, 10);
}

void Snake::clear()
{
    disp->fillRect(Color::BLACK, 50, 50, 10, 10);

}

void Snake::move(int eat)
{
	int head = Snake::getHead();
	int tail = Snake::getTail();
	tile tampon = Snake::getTampon();
	if(!eat)
	{
		tampon[tail].id = 0; // 0 case vide, la queue a avancer
		tail++; // Attention verifier pour le ring buffer
	}

	(*tampon+head)->id = 1; // la case de la tete devient un corps
	switch (Snake::getLastDirection())
		{
		case(North):
				;
		case(East):
				;
		case(South):
				;
		case(West):
				;
		}c
}

void Snake::turn(int direction)
{
	if(direction == North || direction == East || direction == South || direction == West)
		Snake::LastDirection = direction;
}

int Snake::getHead()
{
	return this->head;
}

int Snake::getTail()
{
	return this->tail;
}

tile Snake::getTampon()
{
	return this->tampon[101];
}

int Snake::getLastDirection()
{
	return Snake::LastDirection;
}
}
