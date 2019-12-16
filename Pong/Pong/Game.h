#pragma once
#ifndef GAME_H

#include "SDL.h"
#include <cmath>

class Game {
public:
	Game();

	bool Initialise();

	void RunLoop();

	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	const int thickness = 15;
	const int paddleH = 100;

	Uint32 mTicksCount;

	bool mIsRunning = false;

	struct Vector2 {
		float x;
		float y;
	};

	Vector2 mBallPos;

	Vector2 mLeftPaddlePos;
	int mLeftPaddleDirection;

	Vector2 mRightPaddlePos;
	int mRightPaddleDirection;

	Vector2 mBallVelocity;
};

#endif // !GAME_H
