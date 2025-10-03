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
            return newEntity;
        }
        
        /// @brief Adds an entity into the scene.
        /// @param entity - The entity to add to the scene.
        /// @returns True if submission is successful, false otherwise
        //const bool SubmitEntityToScene(std::shared_ptr<Entity> entity);

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