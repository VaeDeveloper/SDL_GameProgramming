#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "../Logger/Logger.h"

class Registry;

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


    // Hold a pointer to the entity's owner registry
    Registry* registry;
};

class System
{
private:
    Signature componentSignature;
    std::vector<Entity> entities;

public:
    System() = default;
    virtual ~System() = default;

    void AddEntityToSystem(Entity entity);
    void RemoveEntityToSystem(Entity entity);
    std::vector<Entity> GetSystemEntity() const;
    const Signature& GetComponentSignature() const;

    template<typename TComponent> 
    void RequireComponent();
};

/**
 * Interface representing a generic pool. Serves as a base class for typed pools.
 */
class IPool
{
public:
    virtual ~IPool() {}

};

/**
 * A pool class for managing a collection of objects of a specific type.
 *
 * @tparam T The type of objects that the pool will manage.
 */
template<class T>
class Pool : public IPool
{
private:
    /** 
     * A vector to store the objects in the pool.
     */
    std::vector<T> data;

public:
    /**
     * Constructs a Pool with a specified initial size.
     *
     * @param size The initial size of the pool. Defaults to 100.
     */
    Pool(int size = 100)
    {
        data.resize(size);
    }

    /**
     * Virtual destructor for the Pool class.
     */
    virtual ~Pool() = default;

    /**
     * Checks if the pool is empty.
     *
     * @return True if the pool is empty, false otherwise.
     */
    bool IsEmpty() const
    {
        return data.empty();
    }

    /**
     * Gets the current size of the pool.
     *
     * @return The number of objects in the pool.
     */
    int GetSize() const
    {
        return data.size();
    }

    /**
     * Resizes the pool to hold a specified number of objects.
     *
     * @param size The new size of the pool.
     */
    void Resize(int size)
    {
        data.resize(size);
    }

    /**
     * Clears all objects from the pool.
     */
    void Clear()
    {
        data.clear();
    }

    /**
     * Adds an object to the pool.
     *
     * @param object The object to add to the pool.
     */
    void Add(T object)
    {
        data.push_back(object);
    }

    /**
     * Sets the object at a specified index in the pool.
     *
     * @param index The index at which to set the object.
     * @param object The object to set at the specified index.
     */
    void Set(int index, T object)
    {
        data[index] = object;
    }

    /**
     * Gets the object at a specified index in the pool.
     *
     * @param index The index of the object to retrieve.
     * @return A reference to the object at the specified index.
     */
    T& Get(int index) 
    {
        return static_cast<T&>(data[index]);
    }

    /**
     * Provides array-style access to the pool.
     *
     * @param index The index of the object to access.
     * @return A reference to the object at the specified index.
     */
    T& operator[](unsigned int index)
    {
        return data[index];
    }
};

/**
 * The Registry class manages entities, components, and systems within an ECS (Entity-Component-System) architecture.
 */
class Registry
{
private:
    /** 
     * Counter to keep track of the total number of entities.
     */
    int numEntities = 0;

    /** 
     * Stores component pools, where each pool contains components of a specific type for all entities.
     */
    std::vector<std::shared_ptr<IPool>> componentPools;

    /** 
     * Stores a signature for each entity, representing the components associated with the entity.
     */
    std::vector<Signature> entityComponentSignatures;

    /** 
     * Maps component types to their corresponding systems.
     */
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

    /** 
     * Stores entities that need to be added to the registry.
     */
    std::set<Entity> entitiesToBeAdded;

    /** 
     * Stores entities that are scheduled to be removed (killed) from the registry.
     */
    std::set<Entity> entitiesToBeKilled;

public:
    /**
     * Default constructor for the Registry.
     */
    Registry() { Logger::Log("Registry constructor called"); }

    /**
     * Destructor for Registry
     */
    ~Registry() { Logger::Log("Registry destructor called "); }

    /**
     * Updates the registry, processing any pending entity additions or removals.
     */
    void Update();

    /**
     * Creates a new entity and registers it within the registry.
     *
     * @return The newly created entity.
     */
    Entity CreateEntitity();

    /**
     * Adds an entity to the appropriate systems based on its component signature.
     *
     * @param entity The entity to add to relevant systems.
     */
    void AddEntityToSystem(Entity entity);

    /** COMPONENT MANAGER */
    /**
     * Adds a component of type `TComponent` to the specified entity.
     *
     * @tparam TComponent The type of component to add.
     * @tparam TArgs The types of arguments to pass to the component's constructor.
     * @param entity The entity to add the component to.
     * @param args Arguments to construct the component.
     */
    template<class TComponent, class ...TArgs>
    void AddComponent(Entity entity, TArgs&& ...args);

    /**
     * Removes a component of type `TComponent` from the specified entity.
     *
     * @tparam TComponent The type of component to remove.
     * @param entity The entity to remove the component from.
     */
    template<class TComponent>
    void RemoveComponent(Entity entity);

    /**
     * Checks if the specified entity has a component of type `TComponent`.
     *
     * @tparam TComponent The type of component to check.
     * @param entity The entity to check for the component.
     * @return True if the entity has the component, false otherwise.
     */
    template<class TComponent>
    bool HasComponent(Entity entity) const;

    /** SYSTEM MANAGER */
    /**
     * 
     */
    template<class TSystem, class ...TArgs>
    void AddSystem(TArgs&& ...args);

    /**
     * 
     */
    template<class TSystem>
    void RemoveSystem();

    /**
     * 
     */
    template<class TSystem>
    bool HasSystem() const;

    /**
     * 
     */
    template<class TSystem>
    TSystem& GetSystem() const;


    void AddEntityToSystems(Entity entity);
};

/**
 * Sets a requirement for a component type in a system by updating the system's component signature.
 *
 * @tparam TComponent The type of component required by the system.
 */
template <class TComponent>
inline void System::RequireComponent()
{
    const auto componentID = Component<TComponent>::GetID();
    componentSignature.set(componentID);
}


/**
 * Adds a component of type `TComponent` to the specified entity, initializing it with given arguments.
 * Resizes component pools and the pool for this component type if necessary.
 *
 * @tparam TComponent The type of component to add.
 * @tparam TArgs The types of arguments to pass to the component's constructor.
 * @param entity The entity to which the component will be added.
 * @param args Arguments used to construct the component.
 */
template <class TComponent, class... TArgs>
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
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentID] = newComponentPool;
    }

    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentID]);

    if (entityID >= componentPool->GetSize())
    {
        componentPool->Resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityID, newComponent);
    entityComponentSignatures[entityID].set(componentID);
}

/**
 * Removes a component of type `TComponent` from the specified entity.
 *
 * @tparam TComponent The type of component to remove.
 * @param entity The entity from which the component will be removed.
 */
template <class TComponent>
inline void Registry::RemoveComponent(Entity entity)
{
    const auto componentID = Component<TComponent>::GetID();
    const auto entityID = entity.GetID();
    entityComponentSignatures[entityID].set(componentID, false);
}

/**
 * Checks if the specified entity has a component of type `TComponent`.
 *
 * @tparam TComponent The type of component to check.
 * @param entity The entity to check for the component.
 * @return True if the entity has the component, false otherwise.
 */
template <class TComponent>
inline bool Registry::HasComponent(Entity entity) const
{
    const auto componentID = Component<TComponent>::GetID();
    const auto entityID = entity.GetID();

    return entityComponentSignatures[entityID].test(componentID);
}

/**
 * template for add system for registry class 
 */
template <class TSystem, class... TArgs>
inline void Registry::AddSystem(TArgs &&...args)
{
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <class TSystem>
inline void Registry::RemoveSystem()
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template <class TSystem>
inline bool Registry::HasSystem() const
{
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <class TSystem>
inline TSystem &Registry::GetSystem() const
{
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

#endif
