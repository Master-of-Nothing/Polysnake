#ifndef INC_GRAPHOBJECT_H_
#define INC_GRAPHOBJECT_H_

#include "Interfaces/Display/Display.h"
#include <memory>


namespace ELE3312 {

enum tileType
{
	empty = 0,
	snakehead = 1,
	snakebody = 2,
	strawberry = 3,
	blueberry = 4,
	orange = 5,
	banana = 6
};

struct tile
{
	int x;
	int y;
	int id;
};

}

#endif
