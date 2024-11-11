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

void Registry::Update()
{
    // Add the entities that are waiting to be created to the active Systems
    for (auto entity : entitiesToBeAdded)
    {
        AddEntityToSystems(entity);
    }

    entitiesToBeAdded.clear();

}

Entity Registry::CreateEntitity()
{
    int entityId = numEntities++;

    Entity entity(entityId);
    entity.registry= this;

    entitiesToBeAdded.insert(entity);

    if (entityId >= static_cast<int>(entityComponentSignatures.size()))
    {
        entityComponentSignatures.resize(entityId + 1);
    }

    Logger::Log("Entity created with id = " + std::to_string(entityId));

    return entity;

}

void Registry::AddEntityToSystems(Entity entity)
{
    const auto entityID = entity.GetID();
    const auto& entityComponentSignature = entityComponentSignatures[entityID];

    for (auto& system : systems)
    {
        const auto& systemComponentSignature = system.second->GetComponentSignature();
        bool IsInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if (IsInterested)
        {
            system.second->AddEntityToSystem(entity);
        }
    }
}
