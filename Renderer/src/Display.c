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

void DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
	const int deltaX = (x1 - x0);
    const int deltaY = (y1 - y0);

    const int longestSideLength = (abs(deltaX) >= abs(deltaY)) ?               //
                                abs(deltaX) : abs(deltaY);  //


    const float xIncrement = (float)deltaX / (float)longestSideLength;
    const float yIncrement = (float)deltaY / (float)longestSideLength;

    float currentX = (float)x0;
    float currentY = (float)y0;

    for (int i = 0; i < longestSideLength; i++)
    {
        DrawPixel((int)round(currentX), (int)round(currentY), color);
        currentX += xIncrement;
        currentY += yIncrement;
    }
}

void BresenhamLine(int x0, int y0, int x1, int y1, uint32_t color)
{
	const int dx = abs(x1 - x0);
    const int dy = abs(y1 - y0);
    const int sx = (x0 < x1) ? 1 : -1;
    const int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        DrawPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
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

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	DrawLine(x0, y0, x1, y1, color);
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x0, y0, color);
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