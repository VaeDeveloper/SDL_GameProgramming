#ifndef RENDERCOLLIDERSYSTEM_H
#define RENDERCOLLIDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class RenderColliderSystem : public System
{
public:
    RenderColliderSystem()
    {
        RequireComponent<TransformComponent>();
        RequireComponent<BoxCollisionComponent>();
    }

    void Update(SDL_Renderer* renderer)
    {
        for (auto entity : GetSystemEntity())
        {
            const auto transform = entity.GetComponent<TransformComponent>();
            const auto collider = entity.GetComponent<BoxCollisionComponent>();

            SDL_Rect colliderRect = 
            {
                static_cast<int>(transform.position.x + collider.offset.x),
                static_cast<int>(transform.position.y + collider.offset.y),
                static_cast<int>(collider.width),
                static_cast<int>(collider.height)
            };

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &colliderRect);
        }
    }
};

#endif