
#include "Game/Game.h"

using namespace ELE3312;

extern "C"{
	bool FruitCollision_asm(ELE3312::tile* liste_fruit, ELE3312::tile* snake_head, int nombre_fruit);
}
extern "C"{
	bool SnakeCollision_asm(ELE3312::tile* snake_body,int head_index, int longueur_snake);
}

int eat(Snake snake, Fruit fruit)
{
	int LastDirection = snake.getLastDirection();
	int head = snake.getHead();
	tile* tampon = snake.getTampon();
	int eat = 0;
	if(LastDirection == NORTH)
	{
		eat = fruit.find(tampon[head].x,tampon[head].y - TILE_SIZE);
		if(!eat){fruit.clear(tampon[head].x,tampon[head].y - TILE_SIZE);}
		return eat;
	}
	else if (LastDirection == EAST)
	{
		eat = fruit.find(tampon[head].x + TILE_SIZE,tampon[head].y);
		if(!eat){fruit.clear(tampon[head].x + TILE_SIZE,tampon[head].y);}
		return eat;
	}
	else if (LastDirection == SOUTH)
	{
		eat = fruit.find(tampon[head].x,tampon[head].y + TILE_SIZE);
		if(!eat){fruit.clear(tampon[head].x,tampon[head].y + TILE_SIZE);}
		return eat;

	}
	else if (LastDirection == WEST)
	{
		eat = fruit.find(tampon[head].x - TILE_SIZE,tampon[head].y);
		if(!eat){fruit.clear(tampon[head].x - TILE_SIZE,tampon[head].y);}
		return eat;
	}
	return eat;
}

Game::Game(){
	state = Init;
}

void Game::setup(peripheral_handles *handles)
{
	this->handles = handles;
	display.setup(handles->hspi_tft);
	display.clearScreen();
	keypad.setup(handles->gpio_keypad); //On utilise pas le fichier Keypad pour l'instant
	motionInput.setup(handles->hi2c);
	uart.setup(handles->huart);

	Fruit fruit(&display);
	this->fruit = fruit;
	Snake snake(&display);
	Snake snake_opponent(&display);
	this->snake = snake;
	fruit.draw();
	snake.draw();
	snake_opponent.draw();
	uart_payload.player_id = uart.negotiatePlayerId();
	this->state = Running;

}

void Game::run(peripheral_handles *handles)
{
	if(this->state == Init)
		this->setup(handles);
	SnakePayload uart_opponent;
 while(1)
 {
	 if(this->state == Init)
	 		this->setup(handles);
	 if(uart.receiveData(uart_opponent))
	 {
		 snake_opponent.turn(uart_opponent.direction);
	 }
	 keypad.update();
	 if(keypad.keyPress() == KeyCode::FOUR){
		 snake.turn(WEST);
		 uart_payload.direction = WEST;
	 }
	 else{
		 snake.turn(EAST);
		 uart_payload.direction = EAST;
	 }
	 tile *tampon = snake.getTampon();
	 uart_payload.head_x = tampon[snake.getHead()].x;
	 uart_payload.head_y = tampon[snake.getHead()].y;
	 uart.sendData(uart_payload);

	 tile* snake_body = snake.getTampon();
	 if(SnakeCollision_asm(snake_body,snake.getHead(), snake.getLongueur()))
	 	{
	 		state = Init;
	 		display.clearScreen();
	 		continue;
	 	}
	 snake.move(eat(snake,fruit));
	 snake.draw();
	 snake_opponent.draw();
	 HAL_Delay(100);
 }
}

