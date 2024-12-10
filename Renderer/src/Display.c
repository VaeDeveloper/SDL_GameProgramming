#include "Display.h"
#include <stdio.h>

#define BRESENHAM_ALGO 1

SDL_Window* _window = NULL;
SDL_Renderer* _renderer = NULL;
SDL_Texture* _color_buffer_texture = NULL;
uint32_t* _color_buffer = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error Initialize SDL \n ");
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;

	_window = SDL_CreateWindow
	(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);

	if (!_window)
	{
		fprintf(stderr, "Error creating SDL window");
		return false;
	}

	_renderer = SDL_CreateRenderer(_window, -1, 0);

	if (!_renderer) 
	{
		fprintf(stderr, "Error createing SDL renderer");
		return false;
	}

	SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);


	return true;
}

void clear_color_buffer(uint32_t color)
{
	for(int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			_color_buffer[(window_width * y) + x] = color;
		}
	}
}

void draw_debug_grid(void)
{
	for (int y = 0; y < window_height; y += 10)
	{
		for (int x = 0; x < window_width; x += 10)
		{
			if(x % 10 == 0 || y % 10 == 0)
			{
				_color_buffer[(window_width * y) + x]  = 0xFF333333;
			}
		}
	}
}

void draw_pixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < window_width && y >= 0 && y < window_height)
    {
        _color_buffer[(window_width * y) + x] = color;
    }
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
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
        draw_pixel((int)round(currentX), (int)round(currentY), color);
        currentX += xIncrement;
        currentY += yIncrement;
    }
}

void brensenham_line(int x0, int y0, int x1, int y1, uint32_t color)
{
	const int dx = abs(x1 - x0);
    const int dy = abs(y1 - y0);
    const int sx = (x0 < x1) ? 1 : -1;
    const int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        draw_pixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) 
			break;

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

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int currentX = x + i;
            int currentY = y + j;
            draw_pixel(currentX, currentY, color);
        }
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
#if BRESENHAM_ALGO
	brensenham_line(x0, y0, x1, y1, color);
	brensenham_line(x1, y1, x2, y2, color);
	brensenham_line(x2, y2, x0, y0, color);
#else
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x0, y0, color);
#endif
}

void render_color_buffer(void)
{
	SDL_UpdateTexture
	(
		_color_buffer_texture,
		NULL,
		_color_buffer,
		(int)(window_width * sizeof(uint32_t))
	);

	SDL_RenderCopy(_renderer, _color_buffer_texture, NULL, NULL);

}

void destroy_window(void)
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}