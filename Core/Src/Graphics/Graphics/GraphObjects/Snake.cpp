#include "Game/Graphics/GraphObjects/Snake.h"
#include <NucleoImp/Display/ILI9341Display.h>

namespace ELE3312 {

Snake::Snake(ILI9341Display* display)
{
    this->disp = display;
}

void Snake::draw()
{

	disp->fillRect(Color::RED, 50, 50, 10, 10);
}
void Snake::clear()
{
    disp->fillRect(Color::BLACK, 50, 50, 10, 10);
}

}
