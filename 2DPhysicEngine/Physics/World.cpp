#include "World.h"
#include "Constants.h"
#include "CollisionDetection.h"
#include "./Graphics/Graphics.h"
#include <iostream>
#include "./Logger/Logger.h"


World::World(float gravity)
{
    G = -gravity;
    Logger::Log("World constructor called");
}

World::~World()
{
    for (auto body : bodies)
    {
        delete body;
    }
    for (auto constraint : constraints)
    {
        delete constraint;
    }

    Logger::Log("World destructor called");
}

void World::AddBody(Body* body)
{
    bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies()
{
    return bodies;
}

void World::AddConstraint(Constraint* constraint)
{
    constraints.push_back(constraint);
}

std::vector<Constraint*>& World::GetConstraints()
{
    return constraints;
}

void World::AddForce(const Vector2D& force)
{
    forces.push_back(force);
}

void World::AddTorque(float torque)
{
    torques.push_back(torque);
}

void World::Update(float dt)
{
    // Create a vector of penetration constraints that will be solved frame per frame
    std::vector<PenetrationConstraint> penetrations;

    // Loop all bodies of the world applying forces
    for (auto& body : bodies)
    {
        // Apply the weight force to all bodies
        Vector2D weight = Vector2D(0.0, body->mass * G * PhysicEngine::PIXELS_PER_METER);
        body->AddForce(weight);

        // Apply forces to all bodies
        for (auto force : forces)
            body->AddForce(force);

        // Apply torque to all bodies
        for (auto torque : torques)
            body->AddTorque(torque);
    }

    // Integrate all the forces
    for (auto& body : bodies)
    {
        body->IntegrateForces(dt);
    }

    // Check all the bodies with all other bodies detecting collisions
    for (int i = 0; i <= static_cast<int>(bodies.size() - PhysicEngine::POSITIVE); i++)
    {
        for (int j = i + PhysicEngine::POSITIVE; j < static_cast<int>(bodies.size()); j++)
        {
            Body* a = bodies[i];
            Body* b = bodies[j];

            std::vector<Contact> contacts;
            if (CollisionDetection::IsColliding(a, b, contacts))
            {
                for (auto contact : contacts)
                {
                    // Draw collision points
                    Graphics::DrawCircle(contact.start.x, contact.start.y, 5, 0.0, 0xFF00FFFF);
                    Graphics::DrawCircle(contact.end.x, contact.end.y, 2, 0.0, 0xFF00FFFF);

                    // Create a new penetration constraint
                    PenetrationConstraint penetration(contact.a, contact.b, contact.start, contact.end, contact.normal);
                    penetrations.push_back(penetration);
                }
            }
        }
    }

    // Solve all constraints
    for (auto& constraint : constraints)
    {
        constraint->PreSolve(dt);
    }
    for (auto& constraint : penetrations)
    {
        constraint.PreSolve(dt);
    }
    for (int i = PhysicEngine::ZERO; i < 10; i++)
    {
        for (auto& constraint : constraints)
        {
            constraint->Solve();
        }
        for (auto& constraint : penetrations)
        {
            constraint.Solve();
        }
    }
    for (auto& constraint : constraints)
    {
        constraint->PostSolve();
    }
    for (auto& constraint : penetrations)
    {
        constraint.PostSolve();
    }

    // Integrate all the velocities
    for (auto& body : bodies)
    {
        body->IntegrateVelocities(dt);
    }
}
