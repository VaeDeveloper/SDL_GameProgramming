#ifndef SPRITE_H
#define SPRITE_H

#include "Utilities/Deffinitions.h"
#include "Graphics.h"
#include "Player.h"
#include "Textures.h"

/**
 * Sprite struct 
 * x y coord, texture, angle, distance , bool visibility 
 * name is sprite_t
 */
typedef struct
{
    float x;
    float y;
    int texture;
    float angle;
    float distance;
    bool visible;
}sprite_t;


/**
 * Render Sprite Projection function
 */
void RenderSpriteProjection(void);

/**
 * Render Map Sprite function
 */ 
void RenderMapSprites(void);

#endif  // !SPRITE_H
