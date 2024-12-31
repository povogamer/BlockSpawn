#include "Game.h"


int main()
{
	// Init rand seed
	srand(static_cast<unsigned>(time(NULL)));
	// Init Game Engine
	Game game;

	//Game loop
	while(game.running() && !game.getEndGame())
	{
		//Update
		game.update();
		
		//Render
		game.render();
	}

	return 0;
}