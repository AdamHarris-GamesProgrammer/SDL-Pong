#include "Game.h"

Game::Game()
{
	mBallPos.x = 1024 / 2;
	mBallPos.y = 768 / 2;

	mLeftPaddlePos.x = 32;
	mLeftPaddlePos.y = 768 / 2;

	mRightPaddlePos.x = 1024 - 32;
	mRightPaddlePos.y = 768 / 2;

	mBallVelocity.x = -200.0f;
	mBallVelocity.y = 235.0f;

	mTicksCount = 0;
}

bool Game::Initialise()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Pong",	//Window title
		50,		//Top left x-coordinate of the window
		50,		//Top left y-coordinate of the window
		1024,	//Width of the window
		768,	//Height of the window
		0		//Flags (0 for no flags set)
	);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,					//Window that the renderer will be used on
		-1,							//Specifies which graphics driver to use 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC //accelerated means that the game will make use of the GPU, present vsync means that the game will use vsync
	);

	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mIsRunning = true;
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
	SDL_DestroyRenderer(mRenderer);
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

	const Uint8* state = SDL_GetKeyboardState(NULL);
	mLeftPaddleDirection = 0;
	if (state[SDL_SCANCODE_W]) {
		mLeftPaddleDirection -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mLeftPaddleDirection += 1;
	}

	mRightPaddleDirection = 0;
	if (state[SDL_SCANCODE_I]) {
		mRightPaddleDirection -= 1;
	}
	if (state[SDL_SCANCODE_K]) {
		mRightPaddleDirection += 1;
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)); //waits until 16ms has passed before progressing the next iteration

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}

	mUpdatingActors = false;

	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*> deadActors;

	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors) {
		delete actor;
	}

	mTicksCount = SDL_GetTicks();

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	if (mLeftPaddleDirection != 0) {
		mLeftPaddlePos.y += mLeftPaddleDirection * 300.0f * deltaTime;
		if (mLeftPaddlePos.y < (paddleH / 2.0f + thickness)) {
			mLeftPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mLeftPaddlePos.y > (768.0f - paddleH / 2.0f - thickness)) {
			mLeftPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	if (mRightPaddleDirection != 0) {
		mRightPaddlePos.y += mRightPaddleDirection * 300.0f * deltaTime;

		if (mRightPaddlePos.y < (paddleH / 2.0f + thickness)) {
			mRightPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mRightPaddlePos.y > (768.0f - paddleH / 2.0f - thickness)) {
			mRightPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	if (mBallPos.y <= thickness && mBallVelocity.y < 0.0f) {
		mBallVelocity.y *= -1;
	}

	if (mBallPos.y >= 768.0f - thickness && mBallVelocity.y > 0.0f) {
		mBallVelocity.y *= -1;
	}

	mBallPos.x += mBallVelocity.x * deltaTime;
	mBallPos.y += mBallVelocity.y * deltaTime;

	float diffToLeftPaddle = abs(mLeftPaddlePos.y - mBallPos.y);

	if (diffToLeftPaddle <= paddleH / 2.0f && mBallPos.x <= 32.0f && mBallPos.x >= 20.0f && mBallVelocity.x < 0.0f) {
		mBallVelocity.x *= -1.0f;
	}

	float diffToRightPaddle = abs(mRightPaddlePos.y - mBallPos.y);

	if (diffToRightPaddle <= paddleH / 2.0f && mBallPos.x >= (1024.0f - 32.0f) && mBallPos.x <= (1024.0f - 20.0f) && mBallVelocity.x > 0.0f) {
		mBallVelocity.x *= -1.0f;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255); //Sets the screen to pure blue

	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	SDL_Rect topWall{
		0,
		0,
		1024,
		thickness
	};

	SDL_Rect bottomWall{
		0,
		768 - thickness,
		1024,
		thickness
	};

	

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};

	SDL_Rect leftPaddle{
		static_cast<int>(mLeftPaddlePos.x - thickness / 2),
		static_cast<int>(mLeftPaddlePos.y - thickness -25),
		thickness,
		paddleH
	};

	SDL_Rect rightPaddle{
		static_cast<int>(mRightPaddlePos.x - thickness / 2),
		static_cast<int>(mRightPaddlePos.y - thickness - 25),
		thickness,
		paddleH
	};

	SDL_RenderFillRect(mRenderer, &topWall);
	SDL_RenderFillRect(mRenderer, &bottomWall);

	SDL_RenderFillRect(mRenderer, &leftPaddle);
	SDL_RenderFillRect(mRenderer, &rightPaddle);

	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

