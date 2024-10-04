#ifndef SPRITE_H
#define SPRITE_H

#include "Utilities/Deffinitions.h"
#include "Graphics.h"
#include "Player.h"
#include "Textures.h"

typedef struct
{
    float x;
    float y;
    int texture;
    float angle;
    float distance;
    bool visible;
}sprite_t;

void RenderSpriteProjection(void);
void RenderMapSprites(void);

#endif  // !SPRITE_H
