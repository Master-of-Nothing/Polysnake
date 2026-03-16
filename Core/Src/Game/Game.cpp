
#include "Game/Game.h"
#include "NucleoImp/Keypad/KeyPad.h"

namespace ELE3312 {


Game::Game(){}

void Game::setup(peripheral_handles *handles)
{
	this->handles = handles;
	display.setup(handles->hspi_tft);
	display.clearScreen();
	keypad.setup(handles->gpio_keypad); //On utilise pas le fichier Keypad pour l'instant
	motionInput.setup(handles->hi2c);

	Fruit fruit(&display);
	this->fruit = fruit;
	Snake snake(&display);
	this->snake = snake;
	fruit.draw();
	snake.draw();
	this->state = Running;
}

void Game::run(peripheral_handles *handles)
{
	if(this->state == Init)
		this->setup(handles);

 while(1)
 {

	 snake.move(0);
	 snake.draw();
	 HAL_Delay(750);
 }
}


}
