#include "Game.h"
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <SDL2/SDL_image.h>

#include "LevelLoader.h"
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
#include "../Systems/RenderHealthBarSystem.h"
#include "../Systems/RenderGUISystem.h"


#include <imgui/imgui_impl_sdl.h>


#include "../Utilities/Macro.h"

constexpr int LevelNum = 1;

int Game::WindowWidth;
int Game::WindowHeight;
int Game::MapHeight;
int Game::MapWidth;

bool Game::isEditMode = false;
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

	// imgui context 
	ImGui::CreateContext();
	ImGuiSDL::Initialize(renderer, WindowWidth, WindowHeight);


	//initialize the camera
	camera.x = 0;
	camera.y = 0;
	camera.h = WindowHeight;
	camera.w = WindowWidth;

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	isRunning = true;
}

void Game::InitializeSystems()
{
	// registry logic 
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
	registry->AddSystem<RenderHealthBarSystem>();
	registry->AddSystem<RenderGUISystem>();
}

/**
 * 
 */
void Game::Setup()
{
	InitializeSystems();

	LevelLoader loader;
	lua.open_libraries(sol::lib::base, sol::lib::math);
	loader.LoadLevel(lua, renderer, registry, assetManager, 1);
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
		if (!isEditMode) Update();
		
		Render();
		if (isEditMode) registry->GetSystem<RenderGUISystem>().Update(registry);

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
		ImGui_ImplSDL2_ProcessEvent(&event);
		ImGuiIO& io = ImGui::GetIO();

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
		io.MousePos = ImVec2(mouseX, mouseY);
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

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
				if (event.key.keysym.sym == SDLK_p)
				{
					isEditMode = !isEditMode;
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
	registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
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
	registry->GetSystem<RenderHealthBarSystem>().Update(renderer, assetManager, camera);
	
	/** debug box collision from entity */
	if (isDebug)
	{
		registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
		registry->GetSystem<RenderGUISystem>().Update(registry);
	}

	SDL_RenderPresent(renderer);

}

/**
 * Destroy method
 */
void Game::Destroy()
{
	Logger::SaveLogToFile();
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


