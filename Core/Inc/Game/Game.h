#ifndef GAME_H_
#define GAME_H_

#define MaxWidth 320
#define MaxHeigth 240

#define ButtonWidth 120
#define ButtonHeight 40
#define ButtonX (MaxWidth-ButtonWidth)/2
#define ButtonY 100
#define ButtonSpace 60
#define StringHeight 15
#define StringX ButtonX + ButtonWidth/2
#define StringY ButtonY + (ButtonHeigth - StringHeigth)/2

#include "main.h"
#include "cpp_main.h"
#include "Game/Graphics/GraphObjects/Snake.h"
#include "Game/Graphics/GraphObjects/Fruit.h"
#include "Interfaces/Display/Sprite.h"
#include "Interfaces/Display/Display.h"
#include "NucleoImp/Keypad/KeyPad.h"
#include "NucleoImp/SerialCom/UartSnakeManager.h"
#include "NucleoImp/MotionInput/MPU6050MotionInput.h"
#include "NucleoImp/Sound/SnakeAudio.h"



namespace ELE3312{

	enum GameState { Init , Menu , Solo , Multijoueur,  Game_Over };

class Game
{
public :
	Game();
	~Game() = default;
	void setup(peripheral_handles *handles);
	void run(peripheral_handles *handles);
	void update();


private :
	peripheral_handles *handles = nullptr;
	// Périphérique
	ILI9341Display display;
	KeyPad keypad;
	MPU6050MotionInput motionInput;
	SnakeAudio audio;
	//static UartSnakeManager uart;
	Fruit* fruit;
	Snake* snake;
	int moveCommand;
	//static Snake snake_opponent;
	GameState state;
	//GameMode mode;
	//static SnakePayload uart_payload;
	int menuSelection; // 0 = Solo, 1 = Multi

	// Variables pour les options en jeu
	bool useAccelerometer;
	uint32_t gameSpeedDelay;
	uint32_t lastMoveTime;

	float filteredAccX;   // Valeur filtrée de l'accéléromètre X
	float filteredAccY;   // Valeur filtrée de l'accéléromètre Y



	// Nouvelles méthodes internes
	void usingAccelero();
	bool collision();

	void drawMainMenu();
	void drawMenuCursor();
	void updateMenu(KeyCode key);

	void triggerGameOver();
	void updateGameOver(KeyCode key);
	void resetGameObjects();


};



}


#endif
