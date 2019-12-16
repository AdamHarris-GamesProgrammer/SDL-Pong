#pragma once
#ifndef GAME_H

#include "SDL.h"

class Game {
public:
	bool Initialise();

	void RunLoop();

	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;


	bool mIsRunning;
};

#endif // !GAME_H
