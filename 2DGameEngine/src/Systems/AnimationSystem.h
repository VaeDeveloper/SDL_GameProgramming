#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"


class AnimationSystem : public System
{
public:
    AnimationSystem()
    {
        RequireComponent<SpriteComponent>();
        RequireComponent<AnimationComponent>();
    }

    void Update()
    {
        for  (auto entity : GetSystemEntity())
        {
            auto animation = entity.GetComponent<AnimationComponent>();
            auto sprite = entity.GetComponent<SpriteComponent>();


            
        }
    }


};


#endif