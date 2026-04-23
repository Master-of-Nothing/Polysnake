
#include "Game/Game.h"
#include "main.h"
#include "Interfaces/Display/Display.h"
#include "Game/Graphics/GraphObjects/Snake.h"
#include "Game/Graphics/GraphObjects/Fruit.h"


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
	//drawMainMenu();

	menuSelection = 0;
	useAccelerometer = false;
	gameSpeedDelay = 200; // Vitesse initiale (200ms)
	lastMoveTime = HAL_GetTick();

	this->state = Running;

}

void Game::run(peripheral_handles *handles)
{
	if(state == Init)
		setup(handles);
	//SnakePayload uart_opponent;
 while(1)
 {
	 /*drawMainMenu();
	 while(keypad.update() != KeyCode::FIVE)*/ //fonctionne mais a ameliorer

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

void Game::collision() // A compléter
{
	int LastDirection = snake->getLastDirection();
	int head = snake->getHead();
	tile* tampon = snake->getTampon();
	switch(LastDirection)
	{
		case NORTH :
			if(tampon[head].y - TILE_SIZE  < 0)
				////
		case EAST :
			if(tampon[head].x + TILE_SIZE > 320  )
				///
		case SOUTH :
			if(tampon[head].y + TILE_SIZE > 240 )
				///
		case  WEST :
			if(tampon[head].x - TILE_SIZE < 0);
				///
	}
	if(SnakeCollision_asm(snake->getTampon(),snake->getHead(), snake->getLongueur()));
}

void Game::resetGameObjects() {
    // Aucune fuite mémoire, on réinitialise simplement les variables
    snake->reset();
    fruit->reset();
    gameSpeedDelay = 200; // Remet la vitesse par défaut
}

// --- GESTION DE LA FIN DE PARTIE ---

void Game::triggerGameOver() {
    state = Game_Over;
    //audio->setTrack(TRACK_MENU); // Musique calme pour le Game Over -> Non fonctionnel : a faire

    display.clearScreen();
    display.drawString(140, 100, "GAME OVER", Color::RED);//drawString(40, 100, "GAME OVER", COLOR_RED, 3);
    display.drawString(100, 180, "Appuyer sur la Touche 5 pour revenir au Menu", Color::WHITE); // drawString(20, 180, "Touche 5 pour Menu", COLOR_WHITE, 2);
}

void Game::updateGameOver() {
    if (keypad.update() == KeyCode::FIVE) {
        state = Menu; // à revoir
        menuSelection = 0;
        drawMainMenu();
    }
}

// --------- Gestion interface graphique du menu ---------

void Game::drawMainMenu() // décaler l'interface plus au centre
{
	display.clearScreen();

	// Titre
	display.drawString(40, 30, "POLYSNAKE", Color::GREEN);

	// Boutons
	display.drawRect(Color::WHITE,60, 100, 120, 40);
	display.drawString(85, 112, "SOLO",Color::WHITE);

	display.drawRect(Color::WHITE,60, 160, 120, 40);
	display.drawString(75, 172, "MULTI", Color::WHITE);

	drawMenuCursor();
}

void Game::drawMenuCursor() // doit etre decaler vewrs le centre comme l'interface
{
	// Efface l'ancien curseur
	 display.fillRect(Color::BLACK,30, 100, 20, 100);

	 // Dessine le nouveau curseur
	 int yPos = (menuSelection == 0) ? 112 : 172;
	 display.drawString(30, yPos, ">", Color::RED);
}

void Game::updateMenu(KeyCode key)
{
	if (key == KeyCode::EIGHT)
	{
	    menuSelection = 1;
	    drawMenuCursor();
	}
	else if (key == KeyCode::TWO) {
	    menuSelection = 0;
	    drawMenuCursor();
	}
	else if (key == KeyCode::FIVE) {
		display.clearScreen();
	    mode = (menuSelection == 0) ? Solo : Multijoueur;

	    //audio->setTrack(TRACK_GAME); Non fonctionnel à faire
	    resetGameObjects();
	    lastMoveTime = HAL_GetTick(); // Réinitialise le chrono pour le jeu
	}
}

/*
// --- BOUCLE PRINCIPALE ---
void Game::update() {
    KeyCode key = keypad->getKey();

    switch(state) {
        case Menu:
            updateMenu(key);
            break;

		case Multijoeur:
        case Solo:

            // 1. Options (Bascule et Vitesse)
            if (key == KeyCode::THREE) {
                useAccelerometer = !useAccelerometer;
            }
            else if (key == KeyCode::SEVEN) {
               if (gameSpeedDelay < 500) gameSpeedDelay += 20; // Plus lent
            }
            else if (key == KeyCode::NINE) {
                if (gameSpeedDelay > 50) gameSpeedDelay -= 20; // Plus rapide
            }

            // 2. Commandes de mouvement
            Direction moveCommand = NONE; // Assure-toi d'avoir NONE dans ton enum Direction

            if (useAccelerometer) {
                float accelX = motionInput.getX();
                float accelY = motionInput.getY();

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

        case Game_Over:
            updateGameOver(key);
            break;
    }
}*/

