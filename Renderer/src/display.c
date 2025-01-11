#include "display.h"
#include <stdio.h>
#include "utils.h"


extern bool			b_fullscreen;
SDL_Window* 		_window 				= NULL;
SDL_Renderer* 		_renderer 				= NULL;
SDL_Texture* 		_color_buffer_texture 	= NULL;
uint32_t* 			_color_buffer 			= NULL;
int 				window_width 			= 800;
int 				window_height 			= 600;

/** */
void init_display_mode(void){
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width 	= display_mode.w;
	window_height 	= display_mode.h;
}

/** */
bool initialize_window(void){
	check(!SDL_Init(SDL_INIT_EVERYTHING), "error initialize sdl \n");
	init_display_mode();

	_window 		= SDL_CreateWindow(NULL,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
	window_width,window_height,SDL_WINDOW_BORDERLESS);
	check(_window, "error creating sdl_window");

	_renderer 		= SDL_CreateRenderer(_window, -1, 0);
	check(_renderer, "error create sdl_renderer")
	return true;
}

/** */
inline void clear_color_buffer(uint32_t color){
	for(int y = 0; y < window_height; y++){
		for (int x = 0; x < window_width; x++){
			_color_buffer[(window_width * y) + x] = color;
		}
	}
}

/** */
inline void	draw_debug_grid(void){
	for (int y = 0; y < window_height; y += 10){
		for (int x = 0; x < window_width; x += 10){
			if(x % 10 == 0 || y % 10 == 0){
				_color_buffer[(window_width * y) + x]  = 0xFF333333;
			}
		}
	}
}

/** */
inline void draw_pixel(int x, int y, uint32_t color){
    if (x >= 0 && x < window_width && y >= 0 && y < window_height){
        _color_buffer[(window_width * y) + x] = color;
    }
}

/** */
inline void draw_line(int x0, int y0, int x1, int y1, uint32_t color){
    int delta_x 			= (x1 - x0);
    int delta_y 			= (y1 - y0);
    int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);
    float x_inc 			= delta_x / (float)longest_side_length;
    float y_inc 			= delta_y / (float)longest_side_length;
    float current_x 		= x0;
    float current_y 		= y0;

    for (int i = 0; i <= longest_side_length; i++) {
        draw_pixel(round(current_x), round(current_y), color);
        current_x 			+= x_inc;
        current_y 			+= y_inc;
    }
}

/**
 * 
 */
void brensenham_line(int x0, int y0, int x1, int y1, uint32_t color){
	const int 	dx 	= abs(x1 - x0);
    const int 	dy 	= abs(y1 - y0);
    const int 	sx 	= (x0 < x1) ? 1 : -1;
    const int 	sy 	= (y0 < y1) ? 1 : -1;
    int 		err = dx - dy;

    while (true){
        draw_pixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) break;
        
		int 	e2	= 2 * err;

        if (e2 > -dy){
            err 	-= dy;
            x0 		+= sx;
        }
        if (e2 < dx){
            err 	+= dx;
            y0 		+= sy;
        }
    }
}

/** */
void draw_rect(int x, int y, int width, int height, uint32_t color){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

/** */
void render_color_buffer(void){
	SDL_UpdateTexture(_color_buffer_texture,NULL,_color_buffer,(int)(window_width * sizeof(uint32_t)));
	SDL_RenderCopy(_renderer, _color_buffer_texture, NULL, NULL);
}

/** */
void destroy_window(void){
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}