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
    Entity(const Entity& other) = default;
    int GetID() const;

    Entity& operator = (const Entity& other) = default;
    bool operator==(const Entity& other) const { return ID == other.ID; }
    bool operator!=(const Entity& other) const { return ID != other.ID; }
    bool operator<(const Entity& other) const { return ID < other.ID; }
    bool operator>(const Entity& other) const { return ID > other.ID; }

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
    void RemoveEntityToSystem(Entity entity);
    std::vector<Entity> GetSystemEntity() const;
    const Signature& GetComponentSignature() const;

    template<typename TComponent> 
    void RequireComponent();
};

class IPool
{
public:
    virtual ~IPool() {}

};

template<class T>
class Pool : public IPool
{
private:
    std::vector<T> data;

public:
    Pool(int size = 100)
    {
        data.resize(size);
    }

    virtual ~Pool() = default;

    bool IsEmpty() const
    {
        return data.empty();
    }

    int GetSize() const
    {
        return data.size();
    }

    void Resize(int size)
    {
        data.resize(size);
    }

    void Clear()
    {
        data.clear();
    }

    void Add(T object)
    {
        data.add(object);
    }

    void Set(int index, T object)
    {
        data[index] = object;
    }

    T& Get(int index) 
    {
        return static_cast<T&>(data[index]);
    }

    T& operator[](unsigned int index)
    {
        return data[index];
    }
};

class Registry
{
private:
    int numEntity = 0;

public:
    Registry() = default;

    Entity CreateEntity();
    void KillEntity(Entity entity);
    void 
};


template <typename TComponent>
inline void System::RequireComponent()
{
    const auto componentID = Component<TComponent>::GetID();
    componentSignature.set(componentID);
}


#endif


