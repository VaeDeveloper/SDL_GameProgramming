﻿#include "Graphics.h"
#include "Utilities/MacroFunction.h"
#include <stdio.h>

/** Window pointer */
static SDL_Window* _Window = NULL;

/** Renderer pointer */
static SDL_Renderer* _Renderer = NULL;

/** Color buffer */
static color_t* _ColorBuffer = NULL;

/** Texture color Buffer */
static SDL_Texture* _ColorBufferTexture = NULL;

/**
 * Initializes the SDL window and sets up the renderer, color buffer, and texture.
 *
 * This function sets up the necessary components for rendering, including:
 * - SDL initialization
 * - Creating an SDL window
 * - Creating an SDL renderer for drawing
 * - Allocating memory for a color buffer used for rendering
 * - Creating a texture to hold the color buffer in the correct pixel format
 *
 * @return Returns `true` if the initialization is successful. If any SDL functions fail, the program terminates with an error message.
 */
bool InitializeWindow(void)
{
    LOG(stdout, "InitializeApplication\n");
    CHECK(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Error initialize SDL.");

    _Window = SDL_CreateWindow(                  //
        "RaycastEngine",                         //
        SDL_WINDOWPOS_CENTERED,                  //
        SDL_WINDOWPOS_CENTERED,                  //
        WINDOW_WIDTH,                            //
        WINDOW_HEIGHT,                           //
        SDL_WINDOW_FULLSCREEN_DESKTOP);          //
    CHECK(_Window, "Error create SDL window.");  //

    _Renderer = SDL_CreateRenderer(_Window, -1, 0);   //
    CHECK(_Renderer, "Error Creting SDL renderer.");  //

    SDL_SetRenderDrawBlendMode(_Renderer, SDL_BLENDMODE_BLEND);
    CHECK(AllocateColorBuffer(), "Error allocating color buffer");
    CHECK(AllocateColorBufferTexture(), "Error allocating color buffer texture");  //

    return true;
}   

/**
 * Allocates memory for the color buffer.
 * The buffer will store a color value for each pixel on the screen.
 *
 * @return Returns true if the memory allocation is successful, false otherwise.
 */
bool AllocateColorBuffer(void)
{
    /*
     * Allocate memory for the color buffer.
     * The buffer is used to store the pixel data before rendering it to the screen.
     * It holds an array of `color_t` values, one for each pixel on the screen (width * height).
     */
    _ColorBuffer = (color_t*)malloc(sizeof(color_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
    CHECK(_ColorBuffer,"Error Allocating memory for color buffer");
    
    if (_ColorBuffer == NULL)
    {
        LOG(stderr," error color buffer");
        return false;
    }

    return true;
}

/**
 * Creates an SDL texture for the color buffer, which will be used to render the buffer on screen.
 *
 */
bool AllocateColorBufferTexture(void)
{
    _ColorBufferTexture = SDL_CreateTexture(  //
        _Renderer,                            //
        SDL_PIXELFORMAT_RGBA32,               //
        SDL_TEXTUREACCESS_STREAMING,          //
        WINDOW_WIDTH,                         //
        WINDOW_HEIGHT);                       //

    if (_ColorBufferTexture == NULL)
    {
        LOG(stderr, "Error creating color buffer texture.\n");
        return false;
    }

    return true;
}

/**
 *  Destroy Window function
 *  Free color buffer arrray destructor in end application
 */
void DestroyWindow()
{
    LOG(stdout, "Destroy Application ");
    free(_ColorBuffer);
    SDL_DestroyTexture(_ColorBufferTexture);
    SDL_DestroyRenderer(_Renderer);
    SDL_DestroyWindow(_Window);
    SDL_Quit();
}

/**
 * Clear Color buffer  function (for loop color buffer array and set color_t color
 */
void ClearColorBuffer(color_t color)
{
    /* suitable for small buffers, but for large ones it is better to use memset */
    //for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
    //    _ColorBuffer[i] = color;
    /* memset used */
    memset(_ColorBuffer, color, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(color_t));
}

/**
 * Render color buffer array in sdl_ini / render / redner present
 */
void RenderColorBuffer(void)
{
    SDL_UpdateTexture(                           //
        _ColorBufferTexture,                     //
        NULL,                                    //
        _ColorBuffer,                            //
        (int)(WINDOW_WIDTH * sizeof(color_t)));  //

    SDL_RenderCopy(_Renderer, _ColorBufferTexture, NULL, NULL);
    SDL_RenderPresent(_Renderer);
}

/**
 * Draws a pixel at the specified (x, y) coordinates with the given color.
 *
 * @param x     The x-coordinate of the pixel.
 * @param y     The y-coordinate of the pixel.
 * @param color The color to draw the pixel with. It is assumed to be a struct or typedef representing color.
 */
void DrawPixel(int x, int y, color_t color)
{
    _ColorBuffer[(WINDOW_WIDTH * y) + x] = color;
}

/**
 * Draws a rectangle at the specified (x, y) coordinates with the given width, height, and color.
 *
 * @param x      The x-coordinate of the top-left corner of the rectangle.
 * @param y      The y-coordinate of the top-left corner of the rectangle.
 * @param width  The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color  The color to fill the rectangle with.
 */
void DrawRect(int x, int y, int width, int height, color_t color)
{
    for (int i = x; i <= (x + width); i++)
        for (int j = y; j <= (y + height); j++)
            DrawPixel(i, j, color);
}

/**
 * Draws a line between two points (x0, y0) and (x1, y1) using linear interpolation.
 *
 * This function calculates the increments in x and y directions based on the larger
 * difference between the x or y coordinates. It then uses these increments to step
 * along the line and draw pixels at each interpolated point.
 *
 * @param x0    The starting x-coordinate of the line.
 * @param y0    The starting y-coordinate of the line.
 * @param x1    The ending x-coordinate of the line.
 * @param y1    The ending y-coordinate of the line.
 * @param color The color to draw the line with. It is assumed to be a struct or typedef representing color.
 */
void DrawLine(int x0, int y0, int x1, int y1, color_t color)
{

    const int deltaX = (x1 - x0);
    const int deltaY = (y1 - y0);

    /* clang-format off */
    const int longestSideLength = (abs(deltaX) >= abs(deltaY)) ?               //
                                abs(deltaX) : abs(deltaY);  //
    /* clang-format on */

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

/**
 * Draws a line between two points (x0, y0) and (x1, y1) using the Bresenham's line algorithm.
 *
 * This function iteratively calculates the points of the line and draws pixels at the calculated
 * coordinates. It works for all line slopes (positive, negative, steep, shallow).
 *
 * @param x0    The starting x-coordinate of the line.
 * @param y0    The starting y-coordinate of the line.
 * @param x1    The ending x-coordinate of the line.
 * @param y1    The ending y-coordinate of the line.
 * @param color The color to draw the line with. It is assumed to be a struct or typedef representing color.
 */
void BresenhamLine(int x0, int y0, int x1, int y1, color_t color)
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
