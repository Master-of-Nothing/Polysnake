#ifndef GAME_H_
#define GAME_H_

#define MaxWith 200
#define MaxHeigth 100

#include "main.h"
#include "cpp_main.h"
#include "Game/Graphics/GraphObjects/Snake.h"
#include "Interfaces/Display/Sprite.h"

// TODO : Ajouter enum permettant de garder etat du jeu

namespace ELE3312{


class Game
{
public :
	Game();
	~Game() = default;
	void setup(peripheral_handles *handles);
	void run();


private :
	tile Grid[MaxWith][MaxHeigth];
};



}


#endif
