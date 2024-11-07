#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

struct IComponent
{
protected:
    static int NextID;
};

template<class T>
class Component : public IComponent
{
     static int GetID() 
     {
        static auto ID = NextID++;
        return ID;
     }
};

class Entity
{
private:
    int ID;

public:
    Entity(int id) : ID(id){}

    int GetID() const;

};

class System
{
private:
    Signature componentSignature;
    std::vector<Entity> entities;

public:
    System() = default;
    ~System() = default;

    void AddEntityToSystem(Entity entity);
    void RemoveEntityToSystem(Entity Entity);
    std::vector<Entity> GetSystemEntity() const;
    Signature& GetComponentSignature() const;

    template<typename TComponent> 
    void RequireComponent();
};

class Registry
{
};



#endif

template <typename TComponent>
inline void System::RequireComponent()
{
    const auto componentID = Component<TComponent>::GetID();
    componentSignature.set(componentID);
}
