#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector2D.h"

struct Particle {
    int radius;

    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;

    Vector2D sumForces;

    float mass;
    float invMass;

    Particle(float x, float y, float mass);
    ~Particle();

    void AddForce(const Vector2D& force);
    void ClearForces();

    void Integrate(float dt);
};

#endif
