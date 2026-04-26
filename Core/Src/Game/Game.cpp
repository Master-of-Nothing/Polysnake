
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

Game::Game() : state(Init), filteredAccX(0.0f), filteredAccY(0.0f){
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
	//fruit->draw();
	if(this->snake == nullptr)
		this->snake = new Snake(&display);
	//snake->draw();
	//Snake snake_opponent(&display);
	//snake_opponent.draw();
	//uart_payload.player_id = uart.negotiatePlayerId();
	//audio->setTrack(TRACK_MENU); // Lance la musique du menu
	//drawMainMenu();

	menuSelection = 0;
	useAccelerometer = false;
	gameSpeedDelay = 200; // Vitesse initiale (200ms)
	lastMoveTime = HAL_GetTick();

	this->state = Menu;

}

void Game::run(peripheral_handles *handles)
{
	/*if(state == Init)
		setup(handles);
	//SnakePayload uart_opponent;*/
 while(1)
 {
	 KeyCode key = keypad.update();
	 audio.UpdateVolumeFromADC(handles->hadc);
	 switch(state)
	 {
	 case Init :
		 setup(handles);
	 case Menu :
		 drawMainMenu();
		 	 while(keypad.update() != KeyCode::FIVE)
		 	 {
		 		 updateMenu(keypad.update());
		 	 }
		 	 display.clearScreen();
		 	fruit->draw();
		 	snake->draw();

	 case Multijoueur : // Non fonctionnel
		 state = Solo;

	 case Solo :
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
         else if (key == KeyCode::ONE) // Mettre le jeu en pause
         {
        	 while(keypad.update() != KeyCode::ONE){}
        	 key = keypad.update();
         }

         moveCommand = NONE; // Assure-toi d'avoir NONE dans ton enum Direction
                 	 if (useAccelerometer)
                 	 {
                 		 usingAccelero();
                 		/* motionInput.update();
                 		 float accelX = motionInput.getX();
                 		 float accelY = motionInput.getY();
                 		 if (accelX > 0.5f)
                 			 moveCommand = EAST;
                 		 else if (accelX < -0.5f)
                 			 moveCommand = WEST;
                 		 else if (accelY > 0.5f)
                 			 moveCommand = NORTH;
                 		 else if (accelY < -0.5f)
                 			 moveCommand = SOUTH;*/
                 	 } else
                 	 {
                 		 if(key == KeyCode::FOUR)
                 			 moveCommand = WEST;
                 		 else if(key == KeyCode::SIX)
                 			 moveCommand = EAST;
                 	 }

         // 3. Déplacement cadencé par le timer non bloquant
         if ((HAL_GetTick() - lastMoveTime) >= gameSpeedDelay)
         {

        	 if (moveCommand != NONE) {
        		 snake->turn(moveCommand);
        	 }
        	 // Si c'est un demi-tour → on ignore silencieusement
			 // le serpent continue tout droit

             /*if (moveCommand != NONE) {
            	 snake->turn(moveCommand);
             }*/

             snake->move(eat(*snake,*fruit));

             snake->draw();
             //fruit->draw();

             if (collision()) {
                 triggerGameOver();
             }

             lastMoveTime = HAL_GetTick();
         }
         break;
	 case Game_Over:
		 updateGameOver(key);
		 break;
	 }

	/* if(uart.receiveData(uart_opponent))
	 {
		 snake_opponent.turn(uart_opponent.direction);
	 }*/

	/* if(keypad.update() == KeyCode::FOUR)
		 snake->turn(WEST);
	 else if(keypad.update() == KeyCode::SIX)
		 snake->turn(EAST);*/


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


	 /*if(SnakeCollision_asm(snake->getTampon(),snake->getHead(), snake->getLongueur()))
	 	{
	 		state = Init;
	 		display.clearScreen();
	 		continue;
	 	}
	 snake->move(eat(*snake,*fruit));
	 snake->draw();*/
	 //snake_opponent.draw();
	 //HAL_Delay(200);
 }
}

bool Game::collision() // A compléter
{
	int LastDirection = snake->getLastDirection();
	int head = snake->getHead();
	tile* tampon = snake->getTampon();
	switch(LastDirection)
	{
		case NORTH :
			if(tampon[head].y - TILE_SIZE  < 0)
				return 1;////
		case EAST :
			if(tampon[head].x + TILE_SIZE > 320  )
				return 1;///
		case SOUTH :
			if(tampon[head].y + TILE_SIZE > 240 )
				return 1;///
		case  WEST :
			if(tampon[head].x - TILE_SIZE < 0)
				return 1;///
	}
	if(SnakeCollision_asm(snake->getTampon(),snake->getHead(), snake->getLongueur()))
		return 1;
	return 0;
}

void Game::resetGameObjects() {
    // Aucune fuite mémoire, on réinitialise simplement les variables
    snake->reset();
    fruit->reset();
    gameSpeedDelay = 200; // Remet la vitesse par défaut
    // Réinitialise le filtre pour ne pas partir avec une
    // valeur résiduelle de la partie précédente
    filteredAccX = 0.0f;
    filteredAccY = 0.0f;
    useAccelerometer = 0;
}

// --- GESTION DE LA FIN DE PARTIE ---

void Game::triggerGameOver() {
    state = Game_Over;
    //audio->setTrack(TRACK_MENU); // Musique calme pour le Game Over -> Non fonctionnel : a faire

    display.clearScreen();
    display.drawString( (MaxWidth- (9*11))/2 , 100, "GAME OVER", Color::RED);//drawString(40, 100, "GAME OVER", COLOR_RED, 3);
    display.drawString( (MaxWidth- (23*11))/2 , 180, "Appuyer sur la Touche 5", Color::WHITE); // drawString(20, 180, "Touche 5 pour Menu", COLOR_WHITE, 2);
}

void Game::updateGameOver(KeyCode key) {
    if (key == KeyCode::FIVE) {
        state = Menu; // à revoir
        menuSelection = 0;
        display.clearScreen();
        resetGameObjects();
        drawMainMenu();
    }
}

// --------- Gestion interface graphique du menu ---------

void Game::drawMainMenu() // fonctionnel et centré
{
	display.clearScreen();

	// Titre
	display.drawString((MaxWidth-(9*11))/2, ButtonY/2, "POLYSNAKE", Color::GREEN);

	// Boutons
	display.drawRect(Color::WHITE,ButtonX, ButtonY, ButtonWidth, ButtonHeight);
	display.drawString( StringX -( 4*11)/2, 100 + 25/2, "SOLO",Color::WHITE);

	display.drawRect(Color::WHITE,ButtonX, ButtonY + ButtonSpace, ButtonWidth, ButtonHeight);
	display.drawString( StringX - (5*11)/2,160 + 25/2, "MULTI", Color::WHITE);

	drawMenuCursor();
}

void Game::drawMenuCursor()  // fonctionnel et centré
{
	// Efface l'ancien curseur
	 display.fillRect(Color::BLACK,0, 0, 60, 200);

	 // Dessine le nouveau curseur
	 int yPos = (menuSelection == 0) ? 112 : 172;
	 display.drawString(ButtonX/2, yPos, ">", Color::RED);
}

void Game::updateMenu(KeyCode key) // fonctionne parfaitement
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
	    state = (menuSelection == 0) ? Solo : Multijoueur;

	    //audio->setTrack(TRACK_GAME); Non fonctionnel à faire
	    resetGameObjects();
	    lastMoveTime = HAL_GetTick(); // Réinitialise le chrono pour le jeu
	}
}

void Game::usingAccelero()
{
	motionInput.update();
	float rawX = motionInput.getX();
	float rawY = motionInput.getY();

	// === FILTRE PASSE-BAS (lissage exponentiel) ===
	// ALPHA contrôle la réactivité :
	//   - Proche de 1.0 → très réactif mais bruité (comportement actuel)
	//   - Proche de 0.0 → très lisse mais lent
	//   - 0.15 est un bon compromis pour un accéléromètre de jeu
	const float ALPHA = 0.7f;
	filteredAccX = ALPHA * rawX + (1.0f - ALPHA) * filteredAccX;
	filteredAccY = ALPHA * rawY + (1.0f - ALPHA) * filteredAccY;

	// === SEUIL ET AXE DOMINANT ===
	// On utilise les valeurs absolues pour trouver l'axe
	// sur lequel l'inclinaison est la plus prononcée.
	float absX = (filteredAccX < 0.0f) ? -filteredAccX : filteredAccX;
	float absY = (filteredAccY < 0.0f) ? -filteredAccY : filteredAccY;

	// Seuil minimum d'inclinaison pour déclencher un mouvement.
	// À 0.4g, la carte doit être inclinée d'environ 24°.
	// Augmente cette valeur si encore trop sensible (ex: 0.5f ou 0.6f).
	const float THRESHOLD = 0.7f;

	// On ne génère une commande que si l'axe dominant
	// dépasse le seuil. Cela évite les faux positifs
	// en zone neutre (carte quasi-plate).
	if (absX > absY && absX > THRESHOLD) {
		// L'axe X est dominant : mouvement EAST ou WEST
		moveCommand = (filteredAccX > 0.0f) ? EAST : WEST;
	} else if (absY > absX && absY > THRESHOLD) {
		// L'axe Y est dominant : mouvement NORTH ou SOUTH
		moveCommand = (filteredAccY > 0.0f) ? NORTH : SOUTH;
	}
	// Si aucun axe ne dépasse le seuil → moveCommand reste NONE
	// → le serpent continue tout droit

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

