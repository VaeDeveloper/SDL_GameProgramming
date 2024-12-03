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
void DrawRect(int x, int y, int width, int height, uint32_t color);

void RenderColorBuffer(void);
void ClearColorBuffer(uint32_t color);
void DestroyWindow(void);



#endif