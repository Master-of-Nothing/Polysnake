
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

Game::Game() : state(Init), filteredAccX(0.0f), filteredAccY(0.0f), gameSpeedDelay(200),useAccelerometer(false), menuSelection(0), moveCommand(NONE) {
}

void Game::setup(peripheral_handles *handles)
{
	this->handles = handles;
	display.setup(handles->hspi_tft);
	display.clearScreen();
	keypad.setup(handles->gpio_keypad);
	motionInput.setup(handles->hi2c);
	//uart.setup(handles->huart);
	audio.Init(handles->hdac,handles->htim_dac,SnakeAudio::SQUARE);
	audio.setTrack(TRACK_MENU);
	audio.Start();

	if(this->fruit == nullptr)
		this->fruit = new Fruit(&display);
	if(this->snake == nullptr)
		this->snake = new Snake(&display);

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
		 break;
	 case Menu :
		 drawMainMenu();
		 	 while(keypad.update() != KeyCode::FIVE)
		 	 {
		 		 audio.UpdateVolumeFromADC(handles->hadc);
		 		 updateMenu(keypad.update());
		 	 }
		 	 display.clearScreen();
		 	fruit->draw();
		 	snake->draw();
		 	break;

	 case Multijoueur : // Non fonctionnel
		 state = Solo;
		 break;

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

         moveCommand = NONE;
         if (useAccelerometer)
         {
        	 usingAccelero();
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

             snake->move(eat(*snake,*fruit));

             snake->draw();

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
				return 1;
			break;
		case EAST :
			if(tampon[head].x + TILE_SIZE > 320  )
				return 1;
			break;
		case SOUTH :
			if(tampon[head].y + TILE_SIZE > 240 )
				return 1;
			break;
		case  WEST :
			if(tampon[head].x - TILE_SIZE < 0)
				return 1;
			break;
	}
	if(SnakeCollision_asm(snake->getTampon(),snake->getHead(), snake->getLongueur()))
		return 1;
	return 0;
}

void Game::resetGameObjects() {
    // on ne veut pas de fuite mémoire, on réinitialise simplement les variables
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
    audio.setTrack(TRACK_GAME_OVER);

    display.clearScreen();
    display.drawString( (MaxWidth- (9*11))/2 , 100, "GAME OVER", Color::RED);
    display.drawString( (MaxWidth- (23*11))/2 , 180, "Appuyer sur la Touche 5", Color::WHITE);
}

void Game::updateGameOver(KeyCode key) {
    if (key == KeyCode::FIVE) {
        state = Menu;
        menuSelection = 0;
        display.clearScreen();
        resetGameObjects();

        audio.setTrack(TRACK_MENU);

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

	    audio.setTrack(TRACK_GAME);
	    resetGameObjects();
	    lastMoveTime = HAL_GetTick(); // Réinitialise le chrono pour le jeu
	}
}

// --------- Gestion accelerometre du jeu ---------

void Game::usingAccelero()
{
	motionInput.update();
	float rawX = motionInput.getX();
	float rawY = motionInput.getY();

	// FILTRE PASSE-BAS (lissage exponentiel)
	// ALPHA contrôle la réactivité :
	const float ALPHA = 0.7f;
	filteredAccX = ALPHA * rawX + (1.0f - ALPHA) * filteredAccX;
	filteredAccY = ALPHA * rawY + (1.0f - ALPHA) * filteredAccY;

	// SEUIL ET AXE DOMINANT
	// On utilise les valeurs absolues pour trouver l'axe
	// sur lequel l'inclinaison est la plus prononcée.
	float absX = (filteredAccX < 0.0f) ? -filteredAccX : filteredAccX;
	float absY = (filteredAccY < 0.0f) ? -filteredAccY : filteredAccY;

	// Seuil minimum d'inclinaison pour déclencher un mouvement.
	const float THRESHOLD = 0.7f;

	// On ne génère une commande que si l'axe dominant dépasse le seuil.
	if (absX > absY && absX > THRESHOLD) {
		// L'axe X est dominant : mouvement EAST ou WEST
		moveCommand = (filteredAccX > 0.0f) ? WEST : EAST;
	} else if (absY > absX && absY > THRESHOLD) {
		// L'axe Y est dominant : mouvement NORTH ou SOUTH
		moveCommand = (filteredAccY > 0.0f) ? SOUTH : NORTH;
	}

}



