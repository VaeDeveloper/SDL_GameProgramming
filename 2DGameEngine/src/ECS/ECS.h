#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <set>
#include <unordered_map>
#include <typeindex>


/**
 * Defines the maximum number of components that can be registered.
 */
const unsigned int MAX_COMPONENTS = 32;

/**
 * Represents a signature, which is a bitset indicating the presence or absence of components.
 * Each bit corresponds to a specific component type.
 */
using Signature = std::bitset<MAX_COMPONENTS>;

/**
 * Base interface for all components. Contains a static integer that is used to generate unique IDs
 * for each component type.
 */
struct IComponent
{
protected:
    /** 
     * The next available ID for a component. Each new component type will increment this value.
     */
    static int NextID;
};

/**
 * Template class for creating specific component types.
 *
 * @tparam T The component type being created.
 */
template<class T>
class Component : public IComponent
{
   /**
    * Retrieves a unique ID for the component type.
    * 
    * @return A unique ID for the component type.
    */
    static int GetID() 
    {
        static auto ID = NextID++;
        return ID;
    }
};

/**
 * Represents an entity with a unique identifier.
 */
class Entity
{
private:
    /** 
     * Unique identifier for the entity.
     */
    int ID;

public:
/**
     * Constructs an Entity with a specified ID.
     *
     * @param id The unique ID to assign to this entity.
     */
    Entity(int id) : ID(id) {}

    /**
     * Default copy constructor.
     *
     * @param other The entity to copy from.
     */
    Entity(const Entity& other) = default;

    /**
     * Retrieves the unique ID of this entity.
     *
     * @return The ID of this entity.
     */
    int GetID() const;

    /**
     * Default assignment operator.
     *
     * @param other The entity to assign from.
     * @return A reference to this entity.
     */
    Entity& operator=(const Entity& other) = default;

    /**
     * Equality operator to compare two entities by their IDs.
     *
     * @param other The entity to compare with.
     * @return True if the entities have the same ID, false otherwise.
     */
    bool operator==(const Entity& other) const { return ID == other.ID; }

    /**
     * Inequality operator to compare two entities by their IDs.
     *
     * @param other The entity to compare with.
     * @return True if the entities have different IDs, false otherwise.
     */
    bool operator!=(const Entity& other) const { return ID != other.ID; }

    /**
     * Less-than operator to compare two entities by their IDs.
     *
     * @param other The entity to compare with.
     * @return True if this entity's ID is less than the other's ID, false otherwise.
     */
    bool operator<(const Entity& other) const { return ID < other.ID; }

    /**
     * Greater-than operator to compare two entities by their IDs.
     *
     * @param other The entity to compare with.
     * @return True if this entity's ID is greater than the other's ID, false otherwise.
     */
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

    std::vector<IPool*> componentPools;
    std::vector<Signature> entityComponentSignature;
    std::unordered_map<std::type_index, System*> systems;

    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;

public:
    Registry() = default;

    void Update();

    Entity CreateEntitity();
    void AddEntityToSystem(Entity entity);


    template<typename TComponent, typename ...TArgs>
    void AddComponent(Entity entity, TArgs&& ...args);
};


template <typename TComponent>
inline void System::RequireComponent()
{
    const auto componentID = Component<TComponent>::GetID();
    componentSignature.set(componentID);
}

template <typename TComponent, typename... TArgs>
inline void Registry::AddComponent(Entity entity, TArgs &&...args)
{
    const auto componentID = Component<TComponent>::GetID();
    const auto entityID = entity.GetID();

    if (componentID >= componentPools.size())
    {
        componentPools.resize(componentID + 1, nullptr);
    }

    if (!componentPools[componentID])
    {
        Pool<TComponent>* newComponentPool = new Pool<TComponent>();
        componentPools[componentID] = newComponentPool;
    }

    Pool<TComponent>* componentPool = componentPools[componentID];

    if (entityID >= componentPool->GetSize())
    {
        componentPool->Resize(numEntity);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityID, newComponent);
    entityComponentSignature[entityID].set(componentID);
}


#endif
