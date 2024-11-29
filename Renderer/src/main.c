#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool IsRunning = false;
SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;

bool InitializeWindow(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error Initialize SDL \n ");
		return false;
	}

	Window = SDL_CreateWindow
	(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_BORDERLESS
	);

	if (!Window)
	{
		fprintf(stderr, "Error creating SDL window");
		return false;
	}

	Renderer = SDL_CreateRenderer(Window, -1, 0);

	if (!Renderer) 
	{
		fprintf(stderr, "Error createing SDL renderer");
		return false;
	}


	return true;
}

void Setup(void)
{

}

void ProcessInput(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type)
	{
		case SDL_QUIT:
			IsRunning = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				IsRunning = false;
			}

	}
}

void Update(void)
{

}

void Render(void)
{
	SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
	SDL_RenderClear(Renderer);

	SDL_RenderPresent(Renderer);
	
}


int main(void)
{
	IsRunning = InitializeWindow();

	Setup();

	while(IsRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
	return 0;
}

