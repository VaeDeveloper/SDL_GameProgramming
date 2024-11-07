#include "ECS.h"

// TODO implementation function with ECS.h
//
//
int Entity::GetID() const 
{
    return ID;
}

void System::AddEntityToSystem(Entity entity)
{
}

void System::RemoveEntityToSystem(Entity Entity)
{
}

std::vector<Entity> System::GetSystemEntity() const
{
    return std::vector<Entity>();
}

Signature& System::GetComponentSignature() const
{
    // TODO: insert return statement here
}
