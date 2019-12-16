#include "Game.h"

int main(int argc, char* argv[])
{
	Game game;
	
	bool success = game.Initialise();

	if (success) {
		game.RunLoop();
	}
	game.Shutdown();

	return 0;
}