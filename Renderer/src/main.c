#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool IsRunning = false;
SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;
SDL_Texture* ColorBufferTexture = NULL;

uint32_t* ColorBuffer = NULL;

int WindowWidth = 800;
int WindowHeight = 600;


bool InitializeWindow(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error Initialize SDL \n ");
		return false;
	}

	SDL_DisplayMode DisplayMode;
	SDL_GetCurrentDisplayMode(0, &DisplayMode);
	WindowWidth = DisplayMode.w;
	WindowHeight = DisplayMode.h;

	Window = SDL_CreateWindow
	(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WindowWidth,
		WindowHeight,
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

	SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN);


	return true;
}

void Setup(void)
{
	ColorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * WindowWidth * WindowHeight);

	ColorBufferTexture = SDL_CreateTexture
	(
		Renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WindowWidth,
		WindowHeight
	);
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

void ClearColorBuffer(uint32_t color)
{
	for(int y = 0; y < WindowHeight; y++)
	{
		for (int x = 0; x < WindowWidth; x++)
		{
			ColorBuffer[(WindowWidth * y) + x] = color;
		}
	}
}

void Update(void)
{

}

void DrawDebugGrid(void)
{
	for (int y = 0; y < WindowHeight; y += 10)
	{
		for (int x = 0; x < WindowWidth; x += 10)
		{
			if(x % 10 == 0 || y % 10 == 0)
			{
				ColorBuffer[(WindowWidth * y) + x]  = 0xFF333333;
			}
		}
	}
}

void RenderColorBuffer(void)
{
	SDL_UpdateTexture
	(
		ColorBufferTexture,
		NULL,
		ColorBuffer,
		(int)(WindowWidth * sizeof(uint32_t))
	);

	SDL_RenderCopy(Renderer, ColorBufferTexture, NULL, NULL);

}

void Render(void)
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	DrawDebugGrid();

	RenderColorBuffer();
	ClearColorBuffer(0xFF000000);

	SDL_RenderPresent(Renderer);

}

void DestroyWindow(void)
{
	free(ColorBuffer);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
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

	DestroyWindow();

	return 0;
}

