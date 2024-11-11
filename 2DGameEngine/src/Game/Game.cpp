#include "Game.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"


Game::Game()
{
	isRunning = false;
	registry = std::make_unique<Registry>();
	Logger::Log("Game costructor called");
}

Game::~Game()
{
	Logger::Log("Game destructor called");
}

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0 )
	{
		Logger::Err("Error initialization SDL");
		return;
	}
	
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	WindowWidth = displayMode.w;
	WindowHeight =  displayMode.h;
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WindowWidth,
		WindowHeight,
		SDL_WINDOW_BORDERLESS);

	if (!window)
	{
		Logger::Err("Error creating SDL window");
		return;
	}
	
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		Logger::Err("Error creating SDL renderer ");
		return;
	}
	
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	isRunning = true;
}




void Game::Setup()
{
	Entity Tank = registry->CreateEntitity();
	registry->AddComponent<TransformComponent>(Tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);

}

void Game::Run()
{
	Setup();

	while(isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}

}

void Game::ProcessInput()
{
	SDL_Event event;
	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				isRunning = false;
				break;

		
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					isRunning = false;
				}
				break;		
		}
	}
}

void Game::Update()
{

    // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the "previous" frame time
    millisecsPreviousFrame = SDL_GetTicks();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 20, 255);
	SDL_RenderClear(renderer);

	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_RenderPresent(renderer);

}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}




