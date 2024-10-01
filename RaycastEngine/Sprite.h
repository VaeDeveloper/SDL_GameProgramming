#ifndef SPRITE_H
#define SPRITE_H

#include "Utilities/Deffinitions.h"
#include "Graphics.h"
#include "Player.h"
#include "Textures.h"

typedef struct
{
    bool visible;
    float x;
    float y;
    float distance;
    float angle;
    int texture;
}sprite_t;

void RenderSpriteProjection(void);
void RenderMapSprites(void);

#endif  // !SPRITE_H
