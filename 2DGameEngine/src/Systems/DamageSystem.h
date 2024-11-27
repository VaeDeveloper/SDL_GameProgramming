#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/HealthComponent.h"
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
        Entity a = event.a;
        Entity b = event.b;
        Logger::Log("Collision event emitted: " + std::to_string(a.GetID()) + " and " + std::to_string(b.GetID()));
        if (a.BelongsToGroup("projectiles") && b.HasTag("player")) 
        {
            OnProjectileHitsPlayer(a, b); // "a" is the projectile, "b" is the player
        }

        if (b.BelongsToGroup("projectiles") && a.HasTag("player")) 
        {
            OnProjectileHitsPlayer(b, a); // "b" is the projectile, "a" is the player
        }

        if (a.BelongsToGroup("projectiles") && b.BelongsToGroup("enemies")) 
        {
            // TODO: OnProjectileHitsEnemy(...);
        }
        
        if (b.BelongsToGroup("projectiles") && a.BelongsToGroup("enemies")) 
        {
            // TODO: OnProjectileHitsEnemy(...);
        }
    }


    void OnProjectileHitsPlayer(Entity projectile, Entity player)
    {
        auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (!projectileComponent.isFriendly)
        {
            auto& health = player.GetComponent<HealthComponent>();
            health.healthPercentage -= projectileComponent.hitPercentDamage;

            if (health.healthPercentage <= 0)
            {
                player.Kill();
            }

            projectile.Kill();
        }
    }

    
    void Update()
    {

    }
};

#endif