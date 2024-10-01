#ifndef FORCE_H
#define FORCE_H

#include "Vector2D.h"
#include "../Physics/Objects/Body.h"

/*------------------------------------------------------------------------------------------------------------------------------*/
struct Force
{
    static Vector2D GenerateDragForce(const Body& Body, float k);
    static Vector2D GenerateFrictionForce(const Body& Body, float k);
    static Vector2D GenerateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance);
    static Vector2D GenerateSpringForce(const Body& Body, Vector2D anchor, float restLength, float k);
    static Vector2D GenerateSpringForce(const Body& a, const Body& b, float restLength, float k);
};
/*------------------------------------------------------------------------------------------------------------------------------*/

#endif  // !FORCE_H
