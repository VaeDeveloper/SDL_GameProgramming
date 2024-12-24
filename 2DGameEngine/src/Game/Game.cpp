#include "Game.h"
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <SDL2/SDL_image.h>

#include "../Utilities/Utils.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextRenderComponent.h"

#include "../Systems/RenderSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitterSystem.h"
#include "../Systems/ProjectileLifeCycleSystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Utilities/Macro.h"

constexpr int LevelNum = 1;

int Game::WindowWidth;
int Game::WindowHeight;
int Game::MapHeight;
int Game::MapWidth;

/**
 * 
 */
Game::Game()
{
	isRunning = false;
	isDebug = false;
	registry = std::make_unique<Registry>();
	assetManager = std::make_unique<AssetManager>();
	eventBus = std::make_unique<EventBus>();
	Logger::Log("Game costructor called");
}

/**
 * 
 */
Game::~Game()
{
	Logger::Log("Game destructor called");
}

/**
 * 
 */
void Game::Initialize()
{
	Logger::Log("Initialize Game engine");
	check(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Error initialize SDL.");
	check(TTF_Init() == 0, "Error initialize SDL TTF ");
	
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

	check(window, "Error Creating SDL Window");

	renderer = SDL_CreateRenderer(window, -1, 0);
	check(renderer, "Error Creating SDL renderer");

	//initialize the camera
	camera.x = 0;
	camera.y = 0;
	camera.h = WindowHeight;
	camera.w = WindowWidth;

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	isRunning = true;
}

/**
 * 
 */
void Game::LoadLevel(int level)
{
	// unussed params , use in future 
	(void)level;

	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<RenderColliderSystem>();
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyboardControlSystem>();
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<ProjectileEmitterSystem>();
	registry->AddSystem<ProjectileLifeCycleSystem>();
	registry->AddSystem<RenderTextSystem>();

	assetManager->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetManager->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
	assetManager->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
	assetManager->AddTexture(renderer, "tile-image", "./assets/tilemaps/jungle.png");
	assetManager->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");
	assetManager->AddFont("charriot-font", "./assets/fonts/charriot.ttf", 14);
	
	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 60;
	int mapNumRows = 40;
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

			Entity tile = registry->CreateEntity();
			tile.Group("tiles");
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize),
															y * (tileScale * tileSize)),
															glm::vec2(tileScale, tileScale),
															0.0);
			tile.AddComponent<SpriteComponent>("tile-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
		}
	}
	mapFile.close();
	MapWidth = mapNumCols * tileSize * tileScale;
	MapHeight = mapNumRows * tileSize * tileScale;


	Entity chopper = registry->CreateEntity();
	chopper.Tag("player");
	chopper.SetName("Player chopper");
	chopper.AddComponent<TransformComponent>(glm::vec2(237.0, 117.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	chopper.AddComponent<AnimationComponent>(2, 15, true);
	chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 0, 10000, 10, true);
	chopper.AddComponent<BoxCollisionComponent>(32, 25);
	chopper.AddComponent<KeyboardControlledComponent>
	(
		glm::vec2(0, -50), 
		glm::vec2(50, 0), 
		glm::vec2(0, 50), 
		glm::vec2(-50, 0)
	);
	chopper.AddComponent<CameraFollowComponent>();
	chopper.AddComponent<HealthComponent>(1000);


	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(WindowWidth - 75.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 1, true);
	radar.AddComponent<AnimationComponent>(8, 8, true);
	


    Entity tank = registry->CreateEntity();
	tank.Group("enemies");
    tank.AddComponent<TransformComponent>(glm::vec2(500.0,500.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
    tank.AddComponent<BoxCollisionComponent>(32, 32);
    tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000, 3000, 10, false);
    tank.AddComponent<HealthComponent>(100);

    Entity truck = registry->CreateEntity();
	truck.Group("enemies");
    truck.AddComponent<TransformComponent>(glm::vec2(110.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 2);
    truck.AddComponent<BoxCollisionComponent>(32, 32);
    truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0.0, -200.0), 2000, 5000, 10, false);
    truck.AddComponent<HealthComponent>(100);

	Entity label = registry->CreateEntity();
	SDL_Color color {0, 255, 0, 255};
	label.AddComponent<TextRenderComponent>(glm::vec2(WindowWidth / 2 - 40, 10), "CHOPPER 1.0", "charriot-font", color, true);
}

/**
 * 
 */
void Game::Setup()
{
	LoadLevel(LevelNum);
}

/**
 * 
 */
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

/**
 * 
 */
void Game::ProcessInput()
{
	SDL_Event event;
	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
			{
				isRunning = false;
				break;
			}
		
			case SDL_KEYDOWN:
			{
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					isRunning = false;
				}
				if (event.key.keysym.sym == SDLK_d)
				{
					isDebug = !isDebug;
				}

				eventBus->EmitEvent<KeyPressedEvent>(event.key.keysym.sym);
				break;
			}
					
		}
	}
}

void Game::Update()
{
    // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);

    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) 
	{
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the "previous" frame time
    millisecsPreviousFrame = SDL_GetTicks();

	eventBus->Reset();

	// update subscribe to event system (event bus)
	registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
	registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);
	registry->GetSystem<ProjectileEmitterSystem>().SubscribeToEvents(eventBus);

	registry->Update();

	// update allways system 
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update(eventBus);
	registry->GetSystem<CameraMovementSystem>().Update(camera);
	registry->GetSystem<ProjectileEmitterSystem>().Update(registry);
	registry->GetSystem<ProjectileLifeCycleSystem>().Update();

}

/**
 * Render frame function 
 */
void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderSystem>().Update(renderer,assetManager, camera);
	registry->GetSystem<RenderTextSystem>().Update(renderer, assetManager, camera);
	
	/** debug box collision from entity */
	if (isDebug)
	{
		registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
	}

	SDL_RenderPresent(renderer);

}

/**
 * Destroy method
 */
void Game::Destroy()
{
	Logger::SaveLogToFile();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}




