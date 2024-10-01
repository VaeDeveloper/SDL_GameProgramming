#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include "Utilities/Deffinitions.h"
#include "Player.h"

///////////////////////////////////////////////////////////////////////////////////////////
// RAY STRUCT
///////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    bool wasHitVertical;
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    int texture;
} ray_t;

extern ray_t rays[NUM_RAYS];

bool IsRayFacingUp(float angle);
bool IsRayFacingDown(float angle);
bool IsRayFacingLeft(float angle);
bool IsRayFacingRight(float angle);
void CastAllRays(void);
void CastRay(float rayAngle, int stripId);
void RenderMapRays(void);

#endif  // !RAY_H
