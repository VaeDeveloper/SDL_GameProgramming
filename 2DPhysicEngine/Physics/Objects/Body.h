#pragma once


#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "../Vector2D.h"
#include "Shape.h"

struct Body
{
    // Linear motion
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;

    // Forces and torque
    Vector2D sumForces;
    float sumTorque;

    // Mass and Moment of Inertia
    float mass;
    float invMass;
    float I;
    float invI;

    // Coefficient of restitution (elasticity)
    float restitution;

    // Coefficient of friction
    float friction;

    // Pointer to the shape/geometry of this rigid body
    Shape* shape = nullptr;

    // Pointer to an SDL texture
    SDL_Texture* texture = nullptr;

    Body(const Shape& shape, float x, float y, float mass);
    ~Body();

    bool IsStatic() const;

    void AddForce(const Vector2D& force);
    void AddTorque(float torque);
    void ClearForces();
    void ClearTorque();

    void SetTexture(const char* textureFileName);

    Vector2D LocalSpaceToWorldSpace(const Vector2D& point) const;
    Vector2D WorldSpaceToLocalSpace(const Vector2D& point) const;

    void ApplyImpulseLinear(const Vector2D& j);
    void ApplyImpulseAngular(const float j);
    void ApplyImpulseAtPoint(const Vector2D& j, const Vector2D& r);

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    void IntegrateForces(const float dt);
    void IntegrateVelocities(const float dt);
};


