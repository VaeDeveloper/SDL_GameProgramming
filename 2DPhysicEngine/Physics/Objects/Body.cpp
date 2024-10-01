#include "Body.h"
#include <math.h>
#include <iostream>

#include "../Constants.h"

#include "../../Graphics.h"

namespace BodyConstants
{
    float Init_Friction = 0.7;
    float Init_Restitution = 0.6f;
    const float Epsilon = 0.005f;
};  // namespace BodyConstants

/*----------------------------------------------------------------------------------------------------------------*/
Body::Body(const Shape& shape, float x, float y, float mass)
{
    this->shape = shape.Clone();
    this->position = Vector2D(x, y);
    this->velocity = Vector2D::Zero;
    this->acceleration = Vector2D::Zero;
    this->rotation = PhysicEngine::ZERO;
    this->angularVelocity = PhysicEngine::ZERO;
    this->angularAcceleration = PhysicEngine::ZERO;
    this->sumForces = Vector2D::Zero;
    this->sumTorque = PhysicEngine::ZERO;
    this->restitution = BodyConstants::Init_Restitution;
    this->friction = BodyConstants::Init_Friction;
    this->mass = mass;

    if (mass != PhysicEngine::ZERO)
    {
        this->invMass = PhysicEngine::POSITIVE / mass;
    }
    else
    {
        this->invMass = PhysicEngine::ZERO_FLOAT;
    }
    I = shape.GetMomentOfInertia() * mass;
    if (I != PhysicEngine::ZERO_FLOAT)
    {
        this->invI = PhysicEngine::POSITIVE_FLOAT / I;
    }
    else
    {
        this->invI = PhysicEngine::ZERO_FLOAT;
    }
    this->shape->UpdateVertices(rotation, position);
    std::cout << "Body constructor called!" << std::endl;
}
/*----------------------------------------------------------------------------------------------------------------*/
Body::~Body()
{
    delete shape;
    SDL_DestroyTexture(texture);
    std::cout << "Body destructor called!" << std::endl;
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::SetTexture(const char* textureFileName)
{
    SDL_Surface* surface = IMG_Load(textureFileName);
    if (surface)
    {
        texture = SDL_CreateTextureFromSurface(Graphics::Renderer, surface);
        SDL_FreeSurface(surface);
    }
}
/*----------------------------------------------------------------------------------------------------------------*/
bool Body::IsStatic() const
{
    const float epsilon = BodyConstants::Epsilon;
    return fabs(invMass - PhysicEngine::ZERO_FLOAT) < epsilon;
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::AddForce(const Vector2D& force)
{
    sumForces += force;
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::AddTorque(float torque)
{
    sumTorque += torque;
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::ClearForces()
{
    sumForces = Vector2D::Zero;
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::ClearTorque()
{
    sumTorque = PhysicEngine::ZERO_FLOAT;
}
/*----------------------------------------------------------------------------------------------------------------*/
Vector2D Body::LocalSpaceToWorldSpace(const Vector2D& point) const
{
    Vector2D rotated = point.Rotate(rotation);
    return rotated + position;
}
/*----------------------------------------------------------------------------------------------------------------*/
Vector2D Body::WorldSpaceToLocalSpace(const Vector2D& point) const
{
    float translatedX = point.x - position.x;
    float translatedY = point.y - position.y;
    float rotatedX = cos(-rotation) * translatedX - sin(-rotation) * translatedY;
    float rotatedY = cos(-rotation) * translatedY + sin(-rotation) * translatedX;
    return Vector2D(rotatedX, rotatedY);
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::ApplyImpulseLinear(const Vector2D& j)
{
    if (IsStatic()) return;
    velocity += j * invMass;
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::ApplyImpulseAngular(const float j)
{
    if (IsStatic()) return;
    angularVelocity += j * invI;
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::ApplyImpulseAtPoint(const Vector2D& j, const Vector2D& r)
{
    if (IsStatic()) return;
    velocity += j * invMass;
    angularVelocity += r.CrossProduct(j) * invI;
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::IntegrateForces(const float dt)
{
    if (IsStatic()) return;

    // Find the acceleration based on the forces that are being applied and the mass
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Find the angular acceleration based on the torque that is being applied and the moment of inertia
    angularAcceleration = sumTorque * invI;

    // Integrate the angular acceleration to find the new angular velocity
    angularVelocity += angularAcceleration * dt;

    // Clear all the forces and torque acting on the object before the next physics step
    ClearForces();
    ClearTorque();
}
/*----------------------------------------------------------------------------------------------------------------*/
void Body::IntegrateVelocities(const float dt)
{
    if (IsStatic()) return;

    // Integrate the velocity to find the new position
    position += velocity * dt;

    // Integrate the angular velocity to find the new rotation angle
    rotation += angularVelocity * dt;

    // Update the vertices to adjust them to the new position/rotation
    shape->UpdateVertices(rotation, position);
}
/*----------------------------------------------------------------------------------------------------------------*/
