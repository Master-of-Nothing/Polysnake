#include "Game/Graphics/GraphObjects/Snake.h"
#include "Game/Graphics/GraphObjects.h"
#include <NucleoImp/Display/ILI9341Display.h>

#define TILE_SIZE 10

namespace ELE3312 {


Snake::Snake(ILI9341Display* display)
{
    this -> disp = display;
    this -> head = 1;
    this -> tail = 99;
    this -> longueur = this->getLongueur();
    this -> LastDirection = SOUTH;

    int xInit = 10;//(rand() % 31) * 10;
    int yInit = 10;//(rand() % 23) * 10;
    for (int i = 0; i < MaxLength; i++)
        {
        	tampon[i].x = 0;
        	tampon[i].y = 0;
        	tampon[i].id = empty;
        }
    for (int i = 0; i < this->getLongueur(); i++)
    {
    	//printf(" valeur de i dans init : %d",i);
    	int position = this->relativePos(i);
    	tampon[position].x = xInit + (i * TILE_SIZE);
    	tampon[position].y = yInit;
    	tampon[position].id = (position == head) ? snakehead : snakebody;
    }
}

void Snake::draw()
{
	for (int i = 0; i < this->getLongueur(); i++)
	{
    	int position = this->relativePos(i);
		Color color = (position == head) ? Color::DARKGREEN : Color::GREEN ;
		disp->fillRect(color, tampon[position].x, tampon[position].y, TILE_SIZE, TILE_SIZE);
	}
}

void Snake::clearTail() // mets la case de la queue en noir et change son ID pour vide
{
	tampon[tail].id = empty;
    disp->fillRect(Color::BLACK, tampon[tail].x, tampon[tail].y, TILE_SIZE, TILE_SIZE);
    tampon[tail].x = 0;
    tampon[tail].y = 0;
}

void Snake::move(int eat)
{
	int xPos = tampon[head].x;
	int yPos = tampon[head].y;
	if(!eat) // ne mange pas
	{
		Snake::clearTail();
		//tampon[tail].id = empty;
		if((tail +1) % MaxLength) // egale à 0 quand tail = 100
			{
				tail++;
			}
			else
				tail = 0; // Implemtation ring buffer pour tail ; si tail == MaxLength alors on retourne au debut du tableau
	}
	tampon[head].id = snakebody; // la case de la tete devient un corps
	if((head +1) % MaxLength) // egale à 0 quand head = 100
	{
		head++;
	}
	else
		head = 0;
	tampon[head].id = snakehead;
	Snake::newPosition(xPos,yPos);

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
								LastDirection = direction;
							}
						break;
					}
			case(EAST):
					{
						if(direction == NORTH || direction == SOUTH)
							{
								LastDirection = direction;
							}
						break;
					}
			case(SOUTH):
					{
						if(direction == EAST || direction == WEST)
							{
								LastDirection = direction;
							}
						break;
					}
			case(WEST):
					{
						if(direction == NORTH || direction == SOUTH)
							{
								LastDirection = direction;
							}
						break;
					}
			}

}

	//Snake::LastDirection = direction;
int Snake::relativePos(int element) // Renvoie la position de l'un l'élement du corps du serpent par rapport au tableau complet
{
	if(tail + element < MaxLength)
	{
		return ((tail + element)%(MaxLength));
	}
	else
		return (tail + element)%(MaxLength);
}

int Snake::getHead() { return this -> head; }

int Snake::getTail() { return this -> tail; }

tile* Snake::getTampon() { return this->tampon; }

int Snake::getLastDirection() { return Snake::LastDirection; }

int Snake::getLongueur() // Donne les nombre d'élement dans le serpent, sa longueur en soit de 0 à n
{
	if(head > tail)
		{
			return head - tail + 1;
		}
		else
		{
			return (MaxLength - tail + head) + 1;
		}
}
void Snake::newPosition(int xPos, int yPos)
{
	if(LastDirection == NORTH)
			{
				tampon[head].y = yPos - TILE_SIZE;
				tampon[head].x = xPos;
			}
	else if(LastDirection == EAST)
			{
				tampon[head].y = yPos;
				tampon[head].x = xPos + TILE_SIZE;
			}
	else if(LastDirection == SOUTH)
			{
				tampon[head].y = yPos + TILE_SIZE;
				tampon[head].x = xPos;
			}
	else if(LastDirection == WEST)
			{
				tampon[head].y = yPos;
				tampon[head].x = xPos - TILE_SIZE;
			}
}

}
