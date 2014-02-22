/**
 * @file    Manager.h
 * @ingroup ecs
 * @brief   Manage associations of ecs::Entity, ecs::Component and ecs::System.
 *
 * Copyright (c) 2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ecs/Entity.h>
#include <ecs/Component.h>
#include <ecs/ComponentType.h>
#include <ecs/ComponentStore.h>
#include <ecs/System.h>

#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <memory>   // std::shared_ptr
#include <cassert>
#include <limits>
#include <stdexcept>

/**
 * @brief   A simple C++11 Entity-Component-System library.
 * @ingroup ecs
 */
namespace ecs {

/**
 * @brief   Manage associations of Entity, Component and System.
 * @ingroup ecs
 *
 * @todo Map ComponentStore by value, not by pointer.
 * @todo Add a Manager::addSystem() method.
 * @todo Add a Manager::registerEntity() method.
 * @todo Wrap createEntity() -> addComponent() -> registerEntity() methods into a Transaction.
 * @todo Throw instead of returning false in case of error?
 */
class Manager {
public:
    /// Constructor
    Manager();
    /// Destructor
    virtual ~Manager();

    /**
     * @brief   Create a ComponentStore for a certain type of Component.
     * @ingroup ecs
     *
     * @tparam C    A structure derived from Component, of a certain type of Component.
     */
    template<typename C>
    inline bool createComponentStore() {
        static_assert(std::is_base_of<Component, C>::value, "C must derived from the Component struct");
        static_assert(C::_mType != _invalidComponentType, "C must define a valid non-zero _mType");
        return mComponentStores.insert(std::make_pair(C::_mType, IComponentStore::Ptr(new ComponentStore<C>()))).second;
    }

    /**
     * @brief   Get (access to) the ComponentStore of a certain type of Component.
     * @ingroup ecs
     *
     *  Throws std::runtime_error if the ComponentStore does not exist.
     *
     * @tparam C    A structure derived from Component, of a certain type of Component.
     *
     * @return      Reference to the ComponentStore of the specified type (or throws).
     */
    template<typename C>
    inline ComponentStore<C>& getComponentStore() {
        static_assert(std::is_base_of<Component, C>::value, "C must derived from the Component struct");
        static_assert(C::_mType != _invalidComponentType, "C must define a valid non-zero _mType");
        auto iComponentStore = mComponentStores.find(C::_mType);
        if (mComponentStores.end() == iComponentStore) {
            throw std::runtime_error("The ComponentStore does not exist");
        }
        return reinterpret_cast<ComponentStore<C>&>(*(iComponentStore->second));
    }

    /**
     * @brief   Create a new Entity - simply allocate an new Id.
     *
     * @return  Id of the new Entity
     */
    inline Entity createEntity() {
        assert(mLastEntity < std::numeric_limits<Entity>::max());
        return (++mLastEntity);
    }

    /**
     * @brief Add (move) a Component (of the same type as the ComponentStore) associated to an Entity.
     *
     *  Throws std::runtime_error if the ComponentStore does not exist.
     *
     *  Move a new Component into the Store, associating it to its Entity.
     * Using 'rvalue' (using the move semantic of C++11) requires:
     * - calling add() with 'std::move(component)', for instance;
     *   store.add(entity1, ComponentTest1(123);
     * - calling add() with a new temporary 'ComponentXxx(args)', for instance;
     *   ComponentTest1 component1(123);
     *   store.add(entity1, std::move(component1));
     *
     * @tparam C    A structure derived from Component, of a certain type of Component.
     *
     * @param[in] aEntity       Id of the Entity with the Component to add.
     * @param[in] aComponent    'rvalue' to a new Component (of the store type) to add.
     *
     * @return true if insertion succeeded
     */
    template<typename C>
    inline bool addComponent(const Entity aEntity, C&& aComponent) {
        static_assert(std::is_base_of<Component, C>::value, "C must derived from the Component struct");
        static_assert(C::_mType != _invalidComponentType, "C must define a valid non-zero _mType");
        return getComponentStore<C>().add(aEntity, std::move(aComponent));
    }

private:
    /// Id of the last created Entity (start with invalid Id 0).
    Entity                                          mLastEntity;

    /**
     * @brief Hashmap of all registered entities, listing the Type of their Components.
     *
     *  This only associates the Id of each Entity with Types of all it Components.
     * Using a hashmap, since the number of Entities can be very high.
     */
    std::unordered_map<Entity, ComponentTypeSet>    mEntities;

    /**
     * @brief Map of all Components by type and Entity.
     *
     *  Store all Components of each Entity, by ComponentType.
     * Using a standard map, since the number of ComponentType does not usualy grow very high.
     *
     * @todo Map ComponentStore by value, not by pointer.
     */
    std::map<ComponentType, IComponentStore::Ptr>   mComponentStores;

    /**
     * @brief List of all Systems, ordered by insertion (first created, first executed).
     *
     * If a pointer to a System is inserted twice, it is executed twice.
     */
    std::vector<System::Ptr>                        mSystems;
};

} // namespace ecs
