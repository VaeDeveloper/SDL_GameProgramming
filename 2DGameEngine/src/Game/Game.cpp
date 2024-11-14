#include "Game.h"
#include <iostream>
#include <fstream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"

Game::Game()
{
	isRunning = false;
	isDebug = true;
	registry = std::make_unique<Registry>();
	assetManager = std::make_unique<AssetManager>();
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
	WindowHeight = displayMode.h;
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


void Game::LoadLevel(int level)
{
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<RenderColliderSystem>();

	assetManager->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetManager->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
	assetManager->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
	assetManager->AddTexture(renderer, "tile-image", "./assets/tilemaps/jungle.png");

	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 30;
	int mapNumRows = 20;
	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++)
	{
		for (int x = 0; x < mapNumCols; x++)
		{
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntitity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize),
															y * (tileScale * tileSize)),
															glm::vec2(tileScale, tileScale),
															0.0);
			tile.AddComponent<SpriteComponent>("tile-image", tileSize, tileSize, 0, srcRectX, srcRectY);
		}
	}
	mapFile.close();


	Entity chopper = registry->CreateEntitity();
	chopper.SetName("Player chopper");
	chopper.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(23.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 3);
	chopper.AddComponent<AnimationComponent>(2, 25, true);
	chopper.AddComponent<BoxCollisionComponent>(32, 32);

	Entity radar = registry->CreateEntitity();
	radar.AddComponent<TransformComponent>(glm::vec2(WindowWidth - 75.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64,1);
	radar.AddComponent<AnimationComponent>(8, 8, true);


	Entity Tank = registry->CreateEntitity();
	Tank.SetName("Tank 1");
	Tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	Tank.AddComponent<RigidBodyComponent>(glm::vec2(40.0, 0.0));
	Tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
	Tank.AddComponent<BoxCollisionComponent>(32, 32);

	Entity Tank2 = registry->CreateEntitity();
	Tank2.SetName("Tank 2");
	Tank2.AddComponent<TransformComponent>(glm::vec2(100.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	Tank2.AddComponent<RigidBodyComponent>(glm::vec2(-40.0, 0.0));
	Tank2.AddComponent<SpriteComponent>("truck-image", 50, 50, 1);
	Tank2.AddComponent<BoxCollisionComponent>(32, 32);

	Tank.Kill();
}

void Game::Setup()
{
	LoadLevel(1);
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
				if (event.key.keysym.sym == SDLK_d)
				{
					isDebug = !isDebug;
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

	registry->Update();

	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update();

}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 20, 255);
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderSystem>().Update(renderer,assetManager);
	
	if (isDebug)
	{
		registry->GetSystem<RenderColliderSystem>().Update(renderer);
	}

	SDL_RenderPresent(renderer);

}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}




