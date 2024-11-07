#include "ECS.h"
#include <algorithm>


int Entity::GetID() const 
{
    return ID;
}

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntityToSystem(Entity entity)
{
    entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other)
    {
        return entity == other;
    }), entities.end());
}

std::vector<Entity> System::GetSystemEntity() const
{
    return entities;
}

const Signature& System::GetComponentSignature() const
{
    return componentSignature;
}
