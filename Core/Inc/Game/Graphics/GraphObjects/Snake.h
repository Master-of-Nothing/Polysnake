#ifndef SNAKE_H_
#define SNAKE_H_

#define MaxLength 101 // Longueur maximale du serpent

#include <NucleoImp/Display/ILI9341Display.h>
#include "Game/Graphics/GraphObjects.h"

namespace ELE3312 {

struct tile
{
	int x;
	int y;
	int id;
};

enum Direction {North = 1, East = 2, South = 3, West= 4};

class Snake
{
public:
    Snake(ILI9341Display* display);

    ~Snake() = default;
    void draw();
    void clear();
    void move(int eat);
    void turn(int direction);
    int getHead();
    int getTail();
    tile getTampon();
    int getLastDirection();

private:
    ILI9341Display* disp;
    tile tampon[MaxLength];
    int tail;
    int head;
    int LastDirection;
};

}

#endif
