#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "Utilities/Deffinitions.h"
#include "Graphics.h"

#define MAP_NUMBER_ROWS 20
#define MAP_NUMBER_COLS 20


bool MapHasWallAt(float x, float y);
bool IsInsideMap(float x, float y);
int GetMap(int x, int y);
void RenderMapGrid(void);


#endif  // !MAP_H
