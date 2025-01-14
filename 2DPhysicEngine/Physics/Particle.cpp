#include "Particle.h"
#include <iostream>
#include "./Logger/Logger.h"

Particle::Particle(float x, float y, float mass) 
{
    this->position = Vector2D(x, y);
    this->mass = mass;

    if (mass != 0.0) 
    {
        this->invMass = 1.0 / mass;
    } 
    else 
    {
        this->invMass = 0.0;
    }
    Logger::Log("Particle constructor called");
}

Particle::~Particle() 
{
    Logger::Log("Particle destructor called!");
}

void Particle::AddForce(const Vector2D& force) 
{
    sumForces += force;
}

void Particle::ClearForces() 
{
    sumForces = Vector2D(0.0, 0.0);
}

void Particle::Integrate(float dt) 
{
    // Find the acceleration based on the forces that are being applied and the mass
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the acceleration
    position += velocity * dt;

    ClearForces();
}
