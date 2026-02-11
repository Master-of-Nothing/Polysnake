#include "Game/Graphics/GraphObjects/Snake.h"
#include "Game/Graphics/GraphObjects.h"
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
	int xPos = tampon[head].x;
	int yPos = tampon[head].y;
	if(!eat) // ne mange pas
	{
		tampon[this->tail].id = empty; // 0 case vide, la queue a avancer
		Snake::clear();
		if(MaxLength % (tail +1))
			{
				tail++;
			}
			else
				tail = 0; // Implemtation ring buffer pour tail ; si tail == MaxLength alors on retourne au debut du tableau
	}
	tampon[head].id = snakebody; // la case de la tete devient un corps
	if(MaxLength % (head +1))
	{
		head++;
	}
	else
		head = 0;
	tampon[head].id = snakehead;
	switch (LastDirection) // change les position x, y en fonction de la LastDirection
		{
		case(NORTH):
				{
					tampon[head].y = yPos--;
					tampon[head].x = xPos;
				}
		case(EAST):
				{
					tampon[head].y = yPos;
					tampon[head].x = xPos++;
				}
		case(SOUTH):
				{
					tampon[head].y = yPos++;
					tampon[head].x = xPos;
				}
		case(WEST):
				{
					tampon[head].y = yPos;
					tampon[head].x = xPos--;
				}
		}
}

void Snake::turn(int direction)
{
	if(!(direction == NORTH || direction == EAST || direction == SOUTH || direction == WEST))
		return;
	switch (this->LastDirection)
			{
			case(NORTH):
					{
						if(direction == EAST || direction == WEST)
							{
								this-> LastDirection = direction;
							}
						break;
					}
			case(EAST):
					{
						if(direction == NORTH || direction == SOUTH)
							{
								this-> LastDirection = direction;
							}
						break;
					}
			case(SOUTH):
					{
						if(direction == EAST || direction == WEST)
							{
								this-> LastDirection = direction;
							}
						break;
					}
			case(WEST):
					{
						if(direction == NORTH || direction == SOUTH)
							{
								this-> LastDirection = direction;
							}
						break;
					}
			}

}

	//Snake::LastDirection = direction;


int Snake::getHead() { return this -> head; }

int Snake::getTail() { return this -> tail; }

tile* Snake::getTampon() { return this->tampon; }

int Snake::getLastDirection() { return Snake::LastDirection; }

}
