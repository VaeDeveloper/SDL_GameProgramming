#include "Ray.h"
#include "Utilities/Utils.h"

ray_t rays[NUM_RAYS];

#define BRENSEHEM_LINE 1

bool IsRayFacingUp(float angle)
{
    return !IsRayFacingDown(angle);
}

bool IsRayFacingDown(float angle)
{
    return angle > 0 && angle < PI;
}

bool IsRayFacingLeft(float angle)
{
    return !IsRayFacingRight(angle);
}

bool IsRayFacingRight(float angle)
{
    return angle < 0.5 * PI || angle > 1.5 * PI;
}

void CastRay(float rayAngle, int stripId)
{
    NormalizeAngle(&rayAngle);

    float xintercept, yintercept;
    float xstep, ystep;

    ///////////////////////////////////////////
    // HORIZONTAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    bool foundHorzWallHit = false;
    float horzWallHitX = 0;
    float horzWallHitY = 0;
    int horzWallTexture = 0;

    // Find the y-coordinate of the closest horizontal grid intersection
    yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
    yintercept += IsRayFacingDown(rayAngle) ? TILE_SIZE : 0;

    // Find the x-coordinate of the closest horizontal grid intersection
    xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

    // Calculate the increment xstep and ystep
    ystep = TILE_SIZE;
    ystep *= IsRayFacingUp(rayAngle) ? -1 : 1;

    xstep = TILE_SIZE / tan(rayAngle);
    xstep *= (IsRayFacingLeft(rayAngle) && xstep > 0) ? -1 : 1;
    xstep *= (IsRayFacingRight(rayAngle) && xstep < 0) ? -1 : 1;

    float nextHorzTouchX = xintercept;
    float nextHorzTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (IsInsideMap(nextHorzTouchX, nextHorzTouchY))
    {
        float xToCheck = nextHorzTouchX;
        float yToCheck = nextHorzTouchY + (IsRayFacingUp(rayAngle) ? -1 : 0);

        if (MapHasWallAt(xToCheck, yToCheck))
        {
            // found a wall hit
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            horzWallTexture = GetMap((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
            foundHorzWallHit = true;
            break;
        }
        else
        {
            nextHorzTouchX += xstep;
            nextHorzTouchY += ystep;
        }
    }

    ///////////////////////////////////////////
    // VERTICAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    bool foundVertWallHit = false;
    float vertWallHitX = 0;
    float vertWallHitY = 0;
    int vertWallTexture = 0;

    // Find the x-coordinate of the closest vertical grid intersection
    xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
    xintercept += IsRayFacingRight(rayAngle) ? TILE_SIZE : 0;

    // Find the y-coordinate of the closest vertical grid intersection
    yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

    // Calculate the increment xstep and ystep
    xstep = TILE_SIZE;
    xstep *= IsRayFacingLeft(rayAngle) ? -1 : 1;

    ystep = TILE_SIZE * tan(rayAngle);
    ystep *= (IsRayFacingUp(rayAngle) && ystep > 0) ? -1 : 1;
    ystep *= (IsRayFacingDown(rayAngle) && ystep < 0) ? -1 : 1;

    float nextVertTouchX = xintercept;
    float nextVertTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (IsInsideMap(nextVertTouchX, nextVertTouchY))
    {
        float xToCheck = nextVertTouchX + (IsRayFacingLeft(rayAngle) ? -1 : 0);
        float yToCheck = nextVertTouchY;

        if (MapHasWallAt(xToCheck, yToCheck))
        {
            // found a wall hit
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            vertWallTexture = GetMap((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
            foundVertWallHit = true;
            break;
        }
        else
        {
            nextVertTouchX += xstep;
            nextVertTouchY += ystep;
        }
    }

    // Calculate both horizontal and vertical hit distances and choose the smallest one
    float horzHitDistance = foundHorzWallHit ? DistanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY) : FLT_MAX;
    float vertHitDistance = foundVertWallHit ? DistanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY) : FLT_MAX;

    if (vertHitDistance < horzHitDistance)
    {
        rays[stripId].distance = vertHitDistance;
        rays[stripId].wallHitX = vertWallHitX;
        rays[stripId].wallHitY = vertWallHitY;
        rays[stripId].texture = vertWallTexture;
        rays[stripId].wasHitVertical = true;
        rays[stripId].rayAngle = rayAngle;
    }
    else
    {
        rays[stripId].distance = horzHitDistance;
        rays[stripId].wallHitX = horzWallHitX;
        rays[stripId].wallHitY = horzWallHitY;
        rays[stripId].texture = horzWallTexture;
        rays[stripId].wasHitVertical = false;
        rays[stripId].rayAngle = rayAngle;
    }
}

void CastAllRays(void)
{
    for (int col = 0; col < NUM_RAYS; col++)
    {
        float rayAngle = player.rotationAngle + atan((col - NUM_RAYS / 2) / DIST_PROJ_PLANE);
        CastRay(rayAngle, col);
    }
}

void RenderMapRays(void)
{
    for (int i = 0; i < NUM_RAYS; i++)
    {
#if BRENSEHEM_LINE
        // bresengam algorithm for more optimization draw line 
        BresenhamLine(player.x * MINIMAP_SCALE_FACTOR,      //
            player.y * MINIMAP_SCALE_FACTOR,                //
            rays[i].wallHitX * MINIMAP_SCALE_FACTOR,        //
            rays[i].wallHitY * MINIMAP_SCALE_FACTOR,        //
            RED_COLOR                                       //
        );

#else
        // DDA Algorithm standart draw line algorithm 
        DrawLine(player.x * MINIMAP_SCALE_FACTOR,     //
            player.y * MINIMAP_SCALE_FACTOR,          //
            rays[i].wallHitX * MINIMAP_SCALE_FACTOR,  //
            rays[i].wallHitY * MINIMAP_SCALE_FACTOR,  //
            RED_COLOR                                 //
        );
#endif
    }
}
