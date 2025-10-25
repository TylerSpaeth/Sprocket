#ifndef SCENE_H
#define SCENE_H

#include "Core/Macros.h"

#include "Events/Event.h"

#include "Entity.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

    /// @brief Class representing a composed Scene in an application.
    class SPROCKET_API Scene {

        friend class SceneManager;

    private:

        uint64_t m_UUID = UUID::Generate();

        std::function<void(Event&)> m_EventCallback;

        std::vector<std::shared_ptr<Entity>> m_Entities;

    public:

        template<typename T>
        const std::shared_ptr<T> SubmitEntityToScene() {

            if (!std::is_base_of<Entity, T>::value) {
                return nullptr;
            }

            std::shared_ptr<T> newEntity = std::make_shared<T>();
            m_Entities.push_back(newEntity);
            newEntity->InitSelf(newEntity);
            newEntity->m_EventCallback = m_EventCallback;
            if (m_EventCallback) {
                newEntity->OnActivate();
            }
            

            Global::FileLogger().Debug(std::format("Entity ({} - {}) Submitted to Scene ({})", typeid(T).name(), newEntity->m_UUID, m_UUID));

            return newEntity;
        }

        /// @brief Removes an entity from the scene.
        /// @param entity - The entity to be removed from the scene.
        /// @returns True if removal is successful, false otherwise
        const bool RemoveEntityFromScene(std::weak_ptr<Entity> entity);

        /// @brief Assigns the child-parent relation between the given entities. The parent is 
        /// updated to hold the new child. The parent can be null to remove the child's parent.
        /// @brief child - The child entity to have its parent value set
        /// @brief parent - The parent entity to set as the childs parent.
        /// @returns True if assignment is successful, false otherwise
        const bool AssignEntityParent(std::shared_ptr<Entity> child, std::shared_ptr<Entity> parent);

        /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
        /// instead be called directly be the Scene Manager when it recieves an event.
        /// @param event The event the should be handled.
        const void OnEvent(Event& event);

        /// @brief Gets the entity with the given UUID.
        /// @param uuid - The UUID of the entity to get.
        /// @return The entity with the given UUID, or nullptr if not found.
        const std::shared_ptr<Entity> GetEntityByUUID(const uint64_t uuid) const;

        /// @brief Retrieves all entities with the given name.
        /// @param name - The name of the entities to retrieve.
        /// @return A vector of shared pointers to the entities with the given name.
        const std::vector<std::shared_ptr<Entity>> GetEntitiesByName(const std::string& name) const;

        /// @brief Retrieves all entities with the given tag.
        /// @param tag - The tag of the entities to retrieve.
        /// @return A vector of shared pointers to the entities with the given tag.
        const std::vector<std::shared_ptr<Entity>> GetEntitiesByTag(const std::string& tag) const;

    private:

        /// @brief Registers the given function as an Event callback to be run when an event occurs. 
        /// The event handler should subscribe to this in order for events 
        /// produced here to become part of the central event system.
        /// @param eventCallback a function that will take in an Event when an event occurs.
        void RegisterEventCallback(const std::function<void(Event&)> eventCallback);

        /// @brief This is called when the scene becomes the active scene.
        void OnActivate();
        /// @brief This is called when the scene is no long the active scene.
        void OnDeactivate();

        /// @brief This corresponds to application updates.
        /// @param deltaTime The time since the last update
        void OnUpdate(float deltaTime);

    };

}

#endif