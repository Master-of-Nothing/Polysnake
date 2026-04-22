
#include "Game/Game.h"
#include "main.h"



using namespace ELE3312;

extern "C"{
	bool FruitCollision_asm(ELE3312::tile* liste_fruit, ELE3312::tile* snake_head, int nombre_fruit);
}
extern "C"{
	bool SnakeCollision_asm(ELE3312::tile* snake_body,int head_index, int longueur_snake);
}

int eat(Snake& snake, Fruit& fruit)
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

Game::Game() : state(Init){
}

void Game::setup(peripheral_handles *handles)
{
	this->handles = handles;
	display.setup(handles->hspi_tft);
	display.clearScreen();
	keypad.setup(handles->gpio_keypad); //On utilise pas le fichier Keypad pour l'instant
	motionInput.setup(handles->hi2c);
	//uart.setup(handles->huart);
	audio.Init(handles->hdac,handles->htim_dac,SnakeAudio::SQUARE);
	audio.Start();

	if(this->fruit == nullptr)
		this->fruit = new Fruit(&display);
	fruit->draw();
	if(this->snake == nullptr)
		this->snake = new Snake(&display);
	snake->draw();
	//Snake snake_opponent(&display);
	//snake_opponent.draw();
	//uart_payload.player_id = uart.negotiatePlayerId();
	//audio->setTrack(TRACK_MENU); // Lance la musique du menu
	drawMainMenu();

	menuSelection = 0;
	useAccelerometer = false;
	gameSpeedDelay = 200; // Vitesse initiale (200ms)
	lastMoveTime = HAL_GetTick();

	this->state = Running;

}

void Game::run(peripheral_handles *handles)
{
	if(this->state == Init)
		this->setup(handles);
	//SnakePayload uart_opponent;
 while(1)
 {
	 if(this->state == Init)
	 		this->setup(handles);
	/* if(uart.receiveData(uart_opponent))
	 {
		 snake_opponent.turn(uart_opponent.direction);
	 }*/

	 if(keypad.update() == KeyCode::FOUR)
		 snake->turn(WEST);
	 else if(keypad.update() == KeyCode::SIX)
		 snake->turn(EAST);


	/* if(keypad.keyPress() == KeyCode::FOUR){
		 snake->turn(WEST);
		//uart_payload.direction = WEST;
	 }
	 else if(keypad.keyPress() == KeyCode::SIX){
		 snake->turn(EAST);
		 //uart_payload.direction = EAST;
	 }*/
//	 tile *tampon = snake.getTampon();
//	 uart_payload.head_x = tampon[snake->getHead()].x;
//	 uart_payload.head_y = tampon[snake->getHead()].y;
//	 uart.sendData(uart_payload);


	 if(SnakeCollision_asm(snake->getTampon(),snake->getHead(), snake->getLongueur()))
	 	{
	 		state = Init;
	 		display.clearScreen();
	 		continue;
	 	}
	 snake->move(eat(*snake,*fruit));
	 snake->draw();
	 //snake_opponent.draw();
	 HAL_Delay(200);
 }
}

void Game::drawMainMenu()
{
	display->clearScreen();

	// Titre
	display->drawText(40, 30, "POLYSNAKE", COLOR_GREEN, 3);

	// Boutons
	display->drawRect(60, 100, 120, 40, COLOR_WHITE);
	display->drawText(85, 112, "SOLO", COLOR_WHITE, 2);

	display->drawRect(60, 160, 120, 40, COLOR_WHITE);
	display->drawText(75, 172, "MULTI", COLOR_WHITE, 2);

	drawMenuCursor(menuSelection);
}

void Game::drawMenuCursor(int selection)
{
	// Efface l'ancien curseur
	 display->fillRect(30, 100, 20, 100, COLOR_BLACK);

	 // Dessine le nouveau curseur
	 int yPos = (selection == 0) ? 112 : 172;
	 display->drawText(30, yPos, ">", COLOR_RED, 2);
}

void Game::updateMenu(KeyCode key)
{
	if (key == KEY_8)
	{
	    menuSelection = 1;
	    drawMenuCursor(menuSelection);
	}
	else if (key == KEY_2) {
	    menuSelection = 0;
	    drawMenuCursor(menuSelection);
	}
	else if (key == KEY_5) {
		display->clearScreen();
	    currentState = (menuSelection == 0) ? STATE_PLAYING_SOLO : STATE_PLAYING_MULTI;

	    audio->setTrack(TRACK_GAME);
	    resetGameObjects();
	    lastMoveTime = HAL_GetTick(); // Réinitialise le chrono pour le jeu
	}
}

// --- GESTION DE LA FIN DE PARTIE ---
void Game::triggerGameOver() {
    currentState = STATE_GAME_OVER;
    audio->setTrack(TRACK_MENU); // Musique calme pour le Game Over

    display->clearScreen();
    display->drawText(40, 100, "GAME OVER", COLOR_RED, 3);
    display->drawText(20, 180, "Touche 5 pour Menu", COLOR_WHITE, 2);
}

void Game::updateGameOver(KeyCode key) {
    if (key == KEY_5) {
        currentState = STATE_MENU;
        menuSelection = 0;
        drawMainMenu();
    }
}

void Game::resetGameObjects() {
    // Aucune fuite mémoire, on réinitialise simplement les variables
    snake->reset();
    fruit->reset();
    gameSpeedDelay = 200; // Remet la vitesse par défaut
}

// --- BOUCLE PRINCIPALE ---
void Game::update() {
    KeyCode key = keypad->getKey();

    switch(currentState) {
        case STATE_MENU:
            updateMenu(key);
            break;

        case STATE_PLAYING_SOLO:
        case STATE_PLAYING_MULTI:

            // 1. Options (Bascule et Vitesse)
            if (key == KEY_3) {
                useAccelerometer = !useAccelerometer;
            }
            else if (key == KEY_7) {
                gameSpeedDelay += 20; // Plus lent
            }
            else if (key == KEY_9) {
                if (gameSpeedDelay > 40) gameSpeedDelay -= 20; // Plus rapide
            }

            // 2. Commandes de mouvement
            Direction moveCommand = NONE; // Assure-toi d'avoir NONE dans ton enum Direction

            if (useAccelerometer) {
                float accelX = mpu->getAccelX();
                float accelY = mpu->getAccelY();

                if (accelX > 0.5f) moveCommand = RIGHT;
                else if (accelX < -0.5f) moveCommand = LEFT;
                else if (accelY > 0.5f) moveCommand = UP;
                else if (accelY < -0.5f) moveCommand = DOWN;
            } else {
                if (key == KEY_4) moveCommand = LEFT;
                else if (key == KEY_6) moveCommand = RIGHT;
                else if (key == KEY_2) moveCommand = UP;
                else if (key == KEY_8) moveCommand = DOWN;
            }

            // 3. Déplacement cadencé par le timer non bloquant
            if ((HAL_GetTick() - lastMoveTime) >= gameSpeedDelay) {

                if (moveCommand != NONE) {
                    snake->changeDirection(moveCommand);
                }

                snake->move();

                snake->draw(display);
                fruit->draw(display);

                if (snake->checkCollision()) {
                    triggerGameOver();
                }

                lastMoveTime = HAL_GetTick();
            }
            break;

        case STATE_GAME_OVER:
            updateGameOver(key);
            break;
    }
}

