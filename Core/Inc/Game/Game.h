#ifndef GAME_H_
#define GAME_H_

#define MaxWith 200
#define MaxHeigth 100

#include "main.h"
#include "cpp_main.h"
#include "Game/Graphics/GraphObjects/Snake.h"
#include "Game/Graphics/GraphObjects/Fruit.h"
#include "Interfaces/Display/Sprite.h"
#include "NucleoImp/Keypad/KeyPad.h"
#include "NucleoImp/SerialCom/UartSnakeManager.h"
#include <NucleoImp/MotionInput/MPU6050MotionInput.h>



namespace ELE3312{

enum GameState {Init, Running, Victory };

class Game
{
public :
	Game();
	~Game() = default;
	void setup(peripheral_handles *handles);
	void run(peripheral_handles *handles);


private :
	peripheral_handles *handles = nullptr;
	// Périphérique
	static ILI9341Display display;
	static KeyPad keypad;
	static MPU6050MotionInput motionInput;
	static UartSnakeManager uart;
	static Fruit fruit;
	static Snake snake;
	static Snake snake_opponent;
	static GameState state;
	SnakePayload uart_payload;

};



}


#endif
