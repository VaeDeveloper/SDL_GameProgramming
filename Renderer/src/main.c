
#include "Display.h"

bool IsRunning = false;

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

void Update(void)
{

}

void Render(void)
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	DrawDebugGrid();

	DrawPixel(100, 100, 0xFFFF00FF);
	DrawRect(300, 200, 300, 150, 0xFFFF00FF);

	RenderColorBuffer();
	ClearColorBuffer(0xFF000000);

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

	DestroyWindow();

	return 0;
}

