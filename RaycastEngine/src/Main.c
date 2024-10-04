// RaycastEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDL2/SDL.h"
#include <float.h>
#include <stdbool.h>
#include "Graphics.h"
#include "Utilities/MacroFunction.h"
#include "Textures.h"
#include "Map.h"
#include "Ray.h"
#include "Player.h"
#include "Sprite.h"




// on/off for debug minimap 
#define DEBUG_MAP 0

/** Global variable  */ 
int IsGameRunning = false;
int ticksLastFrame = 0;

/**
 * Setup 
 */
void Setup()
{
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(SDL_DISABLE);

    LoadTextures();
}

/**
 * Input Proccessing 
 */
void ProcessInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
        case SDL_QUIT:
        {
            IsGameRunning = false;
            break;
        }
        case SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                IsGameRunning = false;
            }
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
            {
                player.walkDiretion = +1;
            }
            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
            {
                player.walkDiretion = -1;
            }
            if (event.key.keysym.sym == SDLK_a)
            {
                player.strafeDirection = -1;
            }
            if (event.key.keysym.sym == SDLK_d)
            {
                player.strafeDirection = +1;
            }
            if (event.key.keysym.sym == SDLK_LEFT)
            {
                player.turnDirection = -1;
            }
            if (event.key.keysym.sym == SDLK_RIGHT)
            {
                player.turnDirection = +1;
            }
            if (event.key.keysym.sym == SDLK_LSHIFT)
            {
                player.walkSpeed += player.sprintMultiply;
            }
            break;
        }
        case SDL_KEYUP:
        {
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
            {
                player.walkDiretion = 0;
            }
            if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
            {
                player.walkDiretion = 0;
            }
            if (event.key.keysym.sym == SDLK_a)
            {
                player.strafeDirection = 0;
            }
            if (event.key.keysym.sym == SDLK_d)
            {
                player.strafeDirection = 0;
            }
            if (event.key.keysym.sym == SDLK_LEFT)
            {
                player.turnDirection = 0;
            }
            if (event.key.keysym.sym == SDLK_RIGHT)
            {
                player.turnDirection = 0;
            }
            if (event.key.keysym.sym == SDLK_LSHIFT)
            {
                player.walkSpeed -= player.sprintMultiply;
            }
            break;
        }

        //case SDL_MOUSEMOTION: 
        //    player.rotationAngle += event.motion.xrel * player.mouseSensitivity;
        default: break;
    }
}

/**
 * Update Implementation
 */
void Update()
{
    WAIT_FOR_NEXT_FRAME(ticksLastFrame, FRAME_TIME_LENGTH);

    // Waste some time until we reach the target frame time lenght
    float deltaTime = (float)(SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    ticksLastFrame = (float)SDL_GetTicks();

    MovePlayer(deltaTime);
    CastAllRays();
}

/**
 * Render Implementaion 
 */
void Render()
{
    ClearColorBuffer(BLACK_COLOR);
    //RenderWallProjection();
    RenderSpriteProjection();


    // DEBUG Display the minimap
#if DEBUG_MAP
    RenderMapGrid();
    RenderMapRays();
    RenderMapPlayer();
    RenderMapSprites();
#endif  // DEBUG_MAP

    RenderColorBuffer();
}

/**
 * Destruction Resource 
 */
void ReleaseResources(void)
{
    FreeTextures();
    DestroyWindow();
}



/**
 * Main Function 
 */
int main(int argc, char* arga[])
{
    IsGameRunning = InitializeWindow();

    Setup();

    while (IsGameRunning)
    {
        ProcessInput();
        Update();
        Render();
    }

    ReleaseResources();
    return false;
}
///////////////////////////////////////////////////////////////////////////////////////////
