#ifndef FRUIT_H_
#define FRUIT_H_

#include <NucleoImp/Display/ILI9341Display.h>
#include "Game/Graphics/GraphObjects.h"

#define LISTE_FRUITS 10

namespace ELE3312 {

class Fruit
{
public:
    Fruit(ILI9341Display* display);

    ~Fruit() = default;
    void draw();
    void clear(int xPos, int yPos);
    bool find(int xPos, int yPos);
    tile* getFruit();

private:
    ILI9341Display* disp;
    tile fruits[LISTE_FRUITS];
};

}

#endif
