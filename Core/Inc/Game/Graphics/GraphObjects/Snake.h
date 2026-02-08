#ifndef SNAKE_H_
#define SNAKE_H_

#include <NucleoImp/Display/ILI9341Display.h>

namespace ELE3312 {

struct tile
{
	int x;
	int y;
	int id;
};

class Snake
{
public:
    Snake(ILI9341Display* display);

    ~Snake() = default;
    void draw();
    void clear();

private:
    ILI9341Display* disp;
};

}

#endif
