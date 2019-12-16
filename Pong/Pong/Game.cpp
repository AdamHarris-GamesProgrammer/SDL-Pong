#include "Game.h"

bool Game::Initialise()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow(
		"Pong",	//Window title
		50,	//Top left x-coordinate of the window
		50,	//Top left y-coordinate of the window
		1024,	//Width of the window
		768,	//Height of the window
		0		//Flags (0 for no flags set)
	);

	if(!mWindow){
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			break;
		}
	}
}

void Game::UpdateGame()
{

}

void Game::GenerateOutput()
{

}

