#ifndef GAME_H_
#define GAME_H_

#define MaxWith 200
#define MaxHeigth 100

#include "main.h"
#include "cpp_main.h"
#include "Game/Graphics/GraphObjects/Snake.h"
#include "Game/Graphics/GraphObjects/Fruit.h"
#include "Interfaces/Display/Sprite.h"
#include "NucleoImp/Keypad/GPIOKeypad.h"


namespace ELE3312{

enum class GameState {Init, Running, Victory };

class Game
{
public :
	Game();
	~Game() = default;
	void setup(peripheral_handles *handles);
	void run();


private :
	peripheral_handles *handles = nullptr;
	// Périphérique
	static ILI9341Display display;
	static GPIOKeypad Keypad;

	static GameState state;

	tile Grid[MaxWith][MaxHeigth];
};



}


#endif
