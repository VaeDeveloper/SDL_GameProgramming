#ifndef PLAYER_H
#define PLAYER_H

#include "Utilities/Deffinitions.h"
#include "Map.h"
#include "Graphics.h"
#include <float.h>

///////////////////////////////////////////////////////////////////////////////////////////
// PLAYER STRUCT
///////////////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
    int turnDirection;
    int walkDiretion;
    int strafeDirection;

    float x;
    float y;
    float width;
    float height;
    float rotationAngle;
    float walkSpeed;
    float turnSpeed;
    float mouseSensitivity;
    float sprintMultiply;

} player_t;

extern player_t player;


void MovePlayer(float deltaTime);
void RenderMapPlayer(void);

#endif  // !PLAYER_H
