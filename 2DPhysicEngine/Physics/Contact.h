#ifndef CONTACT_H
#define CONTACT_H

#include "Vector2D.h"
#include "../Physics/Objects/Body.h"

struct Contact
{
    Body* a;
    Body* b;

    Vector2D start;
    Vector2D end;

    Vector2D normal;
    float depth;

    Contact() = default;
    ~Contact() = default;

    void ResolvePenetration();
    void ResolveCollision();
};

#endif