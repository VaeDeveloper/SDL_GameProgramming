#ifndef WORLD_H
#define WORLD_H

#include "./Objects/Body.h"
#include "./Constraint.h"
#include <vector>

class World
{
private:
    float G = 9.8f;
    std::vector<Body*> bodies;
    std::vector<Constraint*> constraints;

    std::vector<Vector2D> forces;
    std::vector<float> torques;

public:
    World(float gravity);
    ~World();

    void AddBody(Body* body);
    std::vector<Body*>& GetBodies();

    void AddConstraint(Constraint* constraint);
    std::vector<Constraint*>& GetConstraints();

    void AddForce(const Vector2D& force);
    void AddTorque(float torque);

    void Update(float dt);

    void CheckCollisions();
};

#endif
