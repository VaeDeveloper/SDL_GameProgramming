#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH)
{
    return
    (
        aX < bX + bW &&
        aX + aW > bX &&
        aY < bY + bH &&
        aY + aH > bY
    );
}

class CollisionSystem : public System
{
public:
    CollisionSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxCollisionComponent>();
    }

    void Update()
    {
        auto entities = GetSystemEntity();

        for (auto i = entities.begin(); i != entities.end(); i++)
        {
            Entity a = *i;
            auto aTransform = a.GetComponent<TransformComponent>();
            auto aCollider = a.GetComponent<BoxCollisionComponent>();

            for (auto j = i; j != entities.end(); j++)
            {
                Entity b = *j;

                if (a == b)
                {
                    continue;
                }

                auto bTransform = b.GetComponent<TransformComponent>();
                auto bCollider = b.GetComponent<BoxCollisionComponent>();

                bool collisionHappened = CheckAABBCollision
                (
                    aTransform.position.x + aCollider.offset.x,
                    aTransform.position.y + aCollider.offset.y,
                    aCollider.width,
                    aCollider.height,
                    bTransform.position.x + bCollider.offset.x,
                    bTransform.position.y + bCollider.offset.y,
                    bCollider.width,
                    bCollider.height
                );
                
                if (collisionHappened)
                {
                    Logger::Log("Entity id " + std::to_string(a.GetID()) + " (" + a.GetName() +") " +
                     " is colliding entity id " + std::to_string(b.GetID()) + " (" + b.GetName() + ") ");
                }

            }
        }
    }

};

#endif