#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Contact.h"
#include "../Physics/Objects/Body.h"

struct CollisionDetection
{
    static bool IsColliding(Body* a, Body* b, std::vector<Contact>& contacts);
    static bool IsCollidingCircleCircle(Body* a, Body* b, std::vector<Contact>& contacts);
    static bool IsCollidingPolygonPolygon(Body* a, Body* b, std::vector<Contact>& contacts);
    static bool IsCollidingPolygonCircle(Body* polygon, Body* circle, std::vector<Contact>& contacts);
};
#endif
