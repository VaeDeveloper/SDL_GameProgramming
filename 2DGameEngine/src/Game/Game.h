#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../EventBus/EventBus.h"


const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
	bool isRunning = false;
	bool isDebug = false;
	int millisecsPreviousFrame = 0;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;

	std::unique_ptr<Registry> registry = nullptr;
	std::unique_ptr<AssetManager> assetManager = nullptr;
	std::unique_ptr<EventBus> eventBus = nullptr;

public:
	Game();
	~Game();

	void LoadLevel(int level);

	void Initialize();
	void Setup();	
	void Run();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();
	
	static int WindowWidth;
	static int WindowHeight;
	static int MapWidth;
	static int MapHeight;

};

#endif

