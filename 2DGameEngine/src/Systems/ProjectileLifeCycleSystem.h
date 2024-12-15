#ifndef PROJECTILELIFECYCLESYSTEM_H
#define PROJECTILELIFECYCLESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"
#include <SDL2/SDL.h>


/**
 * 
 */
class ProjectileLifeCycleSystem : public System
{
public:
    /**
     * 
     */
    ProjectileLifeCycleSystem()
    {
        RequireComponent<ProjectileComponent>();
    }

    /**
     * 
     */
    void Update()
    {
        for (auto entity : GetSystemEntity())
        {
            auto projectile = entity.GetComponent<ProjectileComponent>();
            
            if (SDL_GetTicks() - static_cast<Uint32>(projectile.startTime) > static_cast<Uint32>(projectile.duration))
            {
                entity.Kill();
            }
        }
    }


};
#endif