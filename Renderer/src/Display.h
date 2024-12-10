#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)



extern SDL_Window* _window;
extern SDL_Renderer* _renderer;
extern SDL_Texture* _color_buffer_texture;
extern uint32_t* _color_buffer;
extern int window_width;
extern int window_height;

bool initialize_window(void);
void draw_debug_grid(void);
void draw_pixel(int x, int y, uint32_t color);

/**
 * Draws a line from point (x0, y0) to (x1, y1) using linear interpolation.
 *
 * @param x0    The starting x-coordinate of the line.
 * @param y0    The starting y-coordinate of the line.
 * @param x1    The ending x-coordinate of the line.
 * @param y1    The ending y-coordinate of the line.
 * @param color The color to draw the line with. It is assumed to be a struct or typedef representing color.
 */
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

/**
 * Draws a line from point (x0, y0) to (x1, y1) using Bresenham's line algorithm.
 *
 * @param x0    The starting x-coordinate of the line.
 * @param y0    The starting y-coordinate of the line.
 * @param x1    The ending x-coordinate of the line.
 * @param y1    The ending y-coordinate of the line.
 * @param color The color to draw the line with. It is assumed to be a struct or typedef representing color.
 */
void brensenham_line(int x0, int y0, int x1, int y1, uint32_t color);

void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void destroy_window(void);



#endif