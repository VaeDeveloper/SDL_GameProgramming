#include "Display.h"
#include <stdio.h>

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

void DrawPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < WindowWidth && y >= 0 && y < WindowHeight)
    {
        ColorBuffer[(WindowWidth * y) + x] = color;
    }
}

void DrawRect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int currentX = x + i;
            int currentY = y + j;
            DrawPixel(currentX, currentY, color);
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

void DestroyWindow(void)
{
	free(ColorBuffer);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}