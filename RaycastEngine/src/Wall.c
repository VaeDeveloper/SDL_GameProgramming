#include "Wall.h"

/////////////////////////////////////////////////////////////////////////////////////////
// 
// Changes the intensity of a color represented by a color_t pointer,
// modifying its RGB components by a specified factor.
// 
/////////////////////////////////////////////////////////////////////////////////////////
void ChangeColorIntensity(color_t* color, float factor)
{
    unsigned char a = (*color >> 24) & 0xFF;
    unsigned char r = (*color >> 16) & 0xFF;
    unsigned char g = (*color >> 8) & 0xFF;
    unsigned char b = *color & 0xFF;

    r = (unsigned char)(r * factor);
    g = (unsigned char)(g * factor);
    b = (unsigned char)(b * factor);

    *color = (a << 24) | (r << 16) | (g << 8) | b;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
//  Renders a 3D projection of walls onto a 2D screen using raycasting.
//  This function calculates the projected height of each wall segment based on its distance 
//  from the player's viewpoint and the angle of the ray cast in the scene.It then textures 
//  each wall segment based on its hit position and orientation, adjusting colors if the wall 
//  was hit vertically.
// 
/////////////////////////////////////////////////////////////////////////////////////////////
void RenderWallProjection(void)
{
    for (int x = 0; x < NUM_RAYS; x++)
    {
        // Calculate the perpendicular distance to avoid the fish-eye distortion
        float perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);

        // Calculate the projected wall height
        float wallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;

        // Find the wall top Y value
        int wallTopY = (WINDOW_HEIGHT / 2) - (wallHeight / 2);
        wallTopY = wallTopY < 0 ? 0 : wallTopY;

        // Find the wall bottom Y value
        int wallBottomY = (WINDOW_HEIGHT / 2) + (wallHeight / 2);
        wallBottomY = wallBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomY;

        // Draw the ceiling
        for (int y = 0; y < wallTopY; y++)
        {
            DrawPixel(x, y, 0xFF444444);
        }

        // Draw the textured wall
        int textureOffsetX;
        if (rays[x].wasHitVertical)
            textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
        else
            textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;

        // Get the correct texture id number from the map content
        int texNum = rays[x].texture - 1;

        // Query the texture width and height from the upng
        int textureWidth = upng_get_width(_Textures[texNum]);
        int textureHeight = upng_get_height(_Textures[texNum]);

        // Render the wall from wallTopY to wallBottomY
        for (int y = wallTopY; y < wallBottomY; y++)
        {
            int distanceFromTop = y + (wallHeight / 2) - (WINDOW_HEIGHT / 2);
            int textureOffsetY = distanceFromTop * ((float)textureHeight / wallHeight);

            // Set the color of the wall based on the color from the texture
            color_t* wallTextureBuffer = (color_t*)upng_get_buffer(_Textures[texNum]);
            color_t texelColor = wallTextureBuffer[(textureWidth * textureOffsetY) + textureOffsetX];

            // Make the pixel color darker if the ray hit was vertical
            if (rays[x].wasHitVertical)
            {
                ChangeColorIntensity(&texelColor, 0.7);
            }

            DrawPixel(x, y, texelColor);
        }

        // Draw the floor
        for (int y = wallBottomY; y < WINDOW_HEIGHT; y++)
        {
            DrawPixel(x, y, MEDIUM_GRAY_COLOR);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
