#include "ECS.h"
#include <algorithm>

int IComponent::NextID = 0;

int Entity::GetID() const 
{
    return ID;
}

void Entity::Kill()
{
    registry->KillEntity(*this);
    Logger::Log("Kill entity - " + GetName() + " id: " + std::to_string(GetID()));
}

void System::AddEntityToSystem(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity)
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

    //Process entities that are waiting to be killed from the active Systems
    for (auto entity : entitiesToBeKilled)
    {
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entity.GetID()].reset();
        
        //make the entity id available to be reused
        freeIDs.push_back(entity.GetID());
    }

    entitiesToBeKilled.clear();

}

Entity Registry::CreateEntitity()
{
    int entityId;

    if (freeIDs.empty())
    {
        entityId = numEntities++;
        if (entityId >= static_cast<int>(entityComponentSignatures.size()))
        {
            entityComponentSignatures.resize(entityId + 1);
        }
    }
    else
    {
        entityId = freeIDs.front();
        freeIDs.pop_front();        
    }

    Entity entity(entityId);
    entity.registry= this;
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id = " + std::to_string(entityId));

    return entity;

}

void Registry::KillEntity(Entity entity)
{
    entitiesToBeKilled.insert(entity);
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

void Registry::RemoveEntityFromSystems(Entity entity)
{
    for (auto system : systems)
    {
        system.second->RemoveEntityFromSystem(entity);
    }
}
