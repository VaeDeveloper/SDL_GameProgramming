#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window*      _window;                       
extern SDL_Renderer*    _renderer;                     
extern SDL_Texture*     _color_buffer_texture;  
extern uint32_t*        _color_buffer;
extern int              window_width;                            
extern int              window_height;

bool        initialize_window(void);
void        draw_debug_grid(void);
void        draw_pixel(int x, int y, uint32_t color);
void        draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void        brensenham_line(int x0, int y0, int x1, int y1, uint32_t color);
void        draw_rect(int x, int y, int width, int height, uint32_t color);
void        render_color_buffer(void);
void        clear_color_buffer(uint32_t color); 
void        destroy_window(void);



#endif