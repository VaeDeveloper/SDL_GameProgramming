#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/CollisionEvent.h"
#include "../EventBus/EventBus.h"

class DamageSystem : public System
{
public:
    DamageSystem()
    {
        RequireComponent<BoxCollisionComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
    {
        eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
    }

    void OnCollision(CollisionEvent& event)
    {
        Logger::Log("The Damage system received an event collision between entities " + 
        std::to_string(event.a.GetID()) + " and " + std::to_string(event.b.GetID()));

        event.a.Kill();
        event.b.Kill();
    }

    void Update()
    {

    }
};

#endif