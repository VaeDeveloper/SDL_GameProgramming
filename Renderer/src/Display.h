#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern SDL_Texture* ColorBufferTexture;
extern uint32_t* ColorBuffer;
extern int WindowWidth;
extern int WindowHeight;

bool InitializeWindow(void);
void DrawDebugGrid(void);
void DrawPixel(int x, int y, uint32_t color);

/**
 * Draws a line from point (x0, y0) to (x1, y1) using linear interpolation.
 *
 * @param x0    The starting x-coordinate of the line.
 * @param y0    The starting y-coordinate of the line.
 * @param x1    The ending x-coordinate of the line.
 * @param y1    The ending y-coordinate of the line.
 * @param color The color to draw the line with. It is assumed to be a struct or typedef representing color.
 */
void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);

/**
 * Draws a line from point (x0, y0) to (x1, y1) using Bresenham's line algorithm.
 *
 * @param x0    The starting x-coordinate of the line.
 * @param y0    The starting y-coordinate of the line.
 * @param x1    The ending x-coordinate of the line.
 * @param y1    The ending y-coordinate of the line.
 * @param color The color to draw the line with. It is assumed to be a struct or typedef representing color.
 */
void BresenhamLine(int x0, int y0, int x1, int y1, uint32_t color);

void DrawRect(int x, int y, int width, int height, uint32_t color);
void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void RenderColorBuffer(void);
void ClearColorBuffer(uint32_t color);
void DestroyWindow(void);



#endif