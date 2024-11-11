#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"


const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
	bool isRunning = false;
	int millisecsPreviousFrame = 0;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	std::unique_ptr<Registry> registry;
	
public:
	Game();
	~Game();

	void Initialize();
	void Setup();	
	void Run();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();
	
	int WindowWidth;
	int WindowHeight;

};

#endif

