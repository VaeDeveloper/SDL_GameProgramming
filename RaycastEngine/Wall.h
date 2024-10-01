#ifndef WALL_H
#define WALL_H

#include "Utilities/Deffinitions.h"
#include "Ray.h"
#include "Player.h"
#include "Graphics.h"
#include "Textures.h"
#include "upng.h"

void ChangeColorIntensity(color_t* color, float factor);
void RenderWallProjection(void);

#endif  // !WALL_H
