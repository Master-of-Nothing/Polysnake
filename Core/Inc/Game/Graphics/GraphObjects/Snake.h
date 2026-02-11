#ifndef SNAKE_H_
#define SNAKE_H_

#define MaxLength 101 // Longueur maximale du serpent

#include <NucleoImp/Display/ILI9341Display.h>
#include "Game/Graphics/GraphObjects.h"

namespace ELE3312 {

enum {NORTH = 1, EAST = 2, SOUTH = 3, WEST= 4};

class Snake
{
public:
    Snake(ILI9341Display* display);

    ~Snake() = default;
    void draw();
    void clearTail();
    void move(int eat);
    void turn(int direction);
    int getHead();
    int getTail();
    int getLastDirection();
    int getLongueur();
    tile* getTampon();
    int ringBuffer(int i);

private:
    ILI9341Display* disp;
    tile tampon[MaxLength];
    int tail;
    int head;
    int LastDirection;
    int longueur;
};

}

#endif
