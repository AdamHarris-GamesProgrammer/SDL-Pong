#pragma once
#ifndef GAME_H

#include "SDL.h"
#include <cmath>
#include "Actor.h"
#include "Component.h"
#include <vector>
#include "Commons.h"

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

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	const int thickness = 15;
	const int paddleH = 100;

	Uint32 mTicksCount;

	bool mIsRunning = false;
	bool mUpdatingActors;

	Vector2 mBallPos;

	Vector2 mLeftPaddlePos;
	int mLeftPaddleDirection;

	Vector2 mRightPaddlePos;
	int mRightPaddleDirection;

	Vector2 mBallVelocity;

	std::vector<Actor*> mActors;
	std::vector<Actor*> mPendingActors;
};

#endif // !GAME_H
