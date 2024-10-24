#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "Utilities/Deffinitions.h"


/**
 * Initializes the window for rendering.
 *
 * @return Returns `true` if the window was initialized successfully, `false` otherwise.
 */
bool InitializeWindow(void);

/**
 * Allocates memory for the color buffer.
 * The buffer will store a color value for each pixel on the screen.
 */
bool AllocateColorBuffer(void);

/**
 * Creates an SDL texture for the color buffer, which will be used to render the buffer on screen.
 *
 * @return Returns true if the texture is created successfully, false otherwise.
 */
bool AllocateColorBufferTexture(void);

/**
 * Destroys the window and frees any resources associated with it.
 */
void DestroyWindow(void);

/**
 * Destroys the window and frees any resources associated with it.
 */
void ClearColorBuffer(color_t color);

/**
 * Renders the contents of the color buffer to the screen.
 */
void RenderColorBuffer(void);

/**
 * Draws a pixel at the specified (x, y) coordinates with the given color.
 *
 * @param x     The x-coordinate of the pixel.
 * @param y     The y-coordinate of the pixel.
 * @param color The color to draw the pixel with. It is assumed to be a struct or typedef representing color.
 */
void DrawPixel(int x, int y, color_t color);

/**
 * Draws a rectangle at the specified (x, y) coordinates with the given width, height, and color.
 *
 * @param x      The x-coordinate of the top-left corner of the rectangle.
 * @param y      The y-coordinate of the top-left corner of the rectangle.
 * @param width  The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color  The color to fill the rectangle with.
 */
void DrawRect(int x, int y, int width, int height, color_t color);

/**
 * Draws a line from point (x0, y0) to (x1, y1) using linear interpolation.
 *
 * @param x0    The starting x-coordinate of the line.
 * @param y0    The starting y-coordinate of the line.
 * @param x1    The ending x-coordinate of the line.
 * @param y1    The ending y-coordinate of the line.
 * @param color The color to draw the line with. It is assumed to be a struct or typedef representing color.
 */
void DrawLine(int x0, int y0, int x1, int y1, color_t color);

/**
 * Draws a line from point (x0, y0) to (x1, y1) using Bresenham's line algorithm.
 *
 * @param x0    The starting x-coordinate of the line.
 * @param y0    The starting y-coordinate of the line.
 * @param x1    The ending x-coordinate of the line.
 * @param y1    The ending y-coordinate of the line.
 * @param color The color to draw the line with. It is assumed to be a struct or typedef representing color.
 */
void BresenhamLine(int x0, int y0, int x1, int y1, color_t color);


#endif  // ! GRAPHICS_H
