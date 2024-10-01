#include "Map.h"

///////////////////////////////////////////////////////////////////////////////////////////
//// MAP                                                                               ////
///////////////////////////////////////////////////////////////////////////////////////////
static const int map_01[MAP_NUMBER_ROWS][MAP_NUMBER_COLS] = {       //
    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},   //
    {1, 0, 0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 7, 0, 7},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 8, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 1, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},   //
    {1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 2, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 3, 3, 3, 3, 3, 3, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   //
    {1, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   //
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},   //
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};  //
///////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************************************
 *                                                                                         *
 *   MAP HASS Wall AAT                                                                     *
 *  return map[indexY][indexX] size                                                        *
 *                                                                                         *
 ******************************************************************************************/
bool MapHasWallAt(float x, float y)
{
    if (x < 0 || x >= MAP_NUMBER_COLS * TILE_SIZE ||  //
        y < 0 || y >= MAP_NUMBER_ROWS * TILE_SIZE)    //
        return true;                                  //
                                                      //
    int mapGridIndexX = (int)floor(x / TILE_SIZE);    //
    int mapGridIndexY = (int)floor(y / TILE_SIZE);    //

    return map_01[mapGridIndexY][mapGridIndexX] != 0;
}

bool IsInsideMap(float x, float y)
{
    return (x >= 0 && x <= MAP_NUMBER_COLS * TILE_SIZE && y >= 0 && y <= MAP_NUMBER_ROWS * TILE_SIZE);
}

/* render map grid in 2d projection */
void RenderMapGrid(void)
{
    for (int i = 0; i < MAP_NUMBER_ROWS; i++)
    {
        for (int j = 0; j < MAP_NUMBER_COLS; j++)
        {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            color_t tileColor = map_01[i][j] != 0 ?           //
                                    WHITE_COLOR   :           //
                                    FULLY_TRANSPARENT_BLACK;  //

            DrawRect(tileX * MINIMAP_SCALE_FACTOR,  //
                tileY * MINIMAP_SCALE_FACTOR,       //
                TILE_SIZE * MINIMAP_SCALE_FACTOR,   //
                TILE_SIZE * MINIMAP_SCALE_FACTOR,   //
                tileColor);                         //
        }
    }
}
/* getter map */
int GetMap(int x, int y)
{
    return map_01[x][y];
}
