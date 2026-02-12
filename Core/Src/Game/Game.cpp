
#include "Game/Game.h"
#include "NucleoImp/Keypad/GPIOKeypad.h"

namespace ELE3312 {

ILI9341Display Game::display;
GPIOKeypad Game::Keypad;



Game::Game(){}

void Game::setup(peripheral_handles *handles)
{
 this->handles = handles;
 display.setup(handles->hspi_tft);
 display.clearScreen();
 Keypad.setup(handles->gpio_keypad);
// Fruit fruit(&display);
// fruit.draw();
// Snake snake(&display);
// snake.draw();
}

void Game::run()
{
 Fruit fruit(&display);
 fruit.draw();
 Snake snake(&display);
 snake.draw();
 while(1)
 {
	 if(Keypad.isAnyKeyPressed())
	 {
		 switch(Keypad.getFirstKeyPressed())
		 {
		 case KeyCode::TWO:
			 snake.turn(NORTH);
			 break;
		 case KeyCode::SIX:
			 snake.turn(EAST);
			 break;
		 case KeyCode::EIGHT:
		 	 snake.turn(SOUTH);
		 	 break;
		 case KeyCode::FOUR:
		 	 snake.turn(WEST);
		 	 break;
		 default:
			 break;
		 }
	 }
	 snake.move(0);
	 snake.draw();
	 HAL_Delay(750);
 }
}


}
