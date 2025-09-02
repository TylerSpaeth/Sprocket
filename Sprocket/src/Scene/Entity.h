#ifndef ENTITY_H
#define ENTITY_H

#include "Core/Macros.h"

#include "Events/Event.h"

#include "Components/Component.h"
#include "Components/TransformComponent.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

    /// @brief A sort of container for for Components with some extra functionality
    class SPROCKET_API Entity {

        friend class Scene;

    private:

        std::function<void(Event&)> m_EventCallback;

        Entity* m_Parent = nullptr;

        std::vector<Entity*> m_Children;

        std::vector<Component*> m_Components;

        TransformComponent m_Transform;

        // Returns a transform representing the global of this entities parent.. The local values
        // are all the matter on this component. Global values are invalid.
        TransformComponent GetParentGlobalTransform();

        /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
        /// instead be called directly be the Scene when it recieves an event.
        /// @param event The event the should be handled.
        void OnEvent(Event& event);

        // Note that these three functions below are called by the scene that they are a part of. 
        // As part of each of these functions, they will call Start(), Update(), and End() which 
        // are the public, overridable functions meant for user customization of the entities 
        // behavior.

        /// @brief This is called when the scene this entity is a part of becomes active or this
        /// entity is added to an active scene
        void OnActivate();
        /// @brief This is called when the scene this entity is a part of is no longer the active 
        /// scene or this entity is removed from an active scene
        void OnDeactivate();
        /// @brief This corresponds to application updates.
        /// @param deltaTime The time since the last update
        void OnUpdate(float deltaTime);

    public:

        Entity();
        ~Entity();

        virtual void Start() {}
        virtual void Update(float deltaTime) {}
        virtual void End() {}

        // Only one of each component type is allowed on a single Entity
        template<typename T>
        bool AddComponent() {
            // Iterate over all of the components and try to static cast them to be the type of 
            // component we are looking for. If the static cast is not null then we already
            // have this type of component.
            for (Component* component : m_Components) {
                T* existingComponent = dynamic_cast<T*>(component);
                if (existingComponent != nullptr) {
                    return false;
                }
            }
            m_Components.push_back(new T());
            return true;
        }

        template<typename T>
        T* GetComponent() {
            // Iterate over all of the components and try to dynamic cast them to be the type of 
            // component we are looking for. If the static cast in not null then we have found
            // the component
            for (Component* component : m_Components) {
                T* existingComponent = dynamic_cast<T*>(component);
                if (existingComponent != nullptr) {
                    return existingComponent;
                }
            }
            return nullptr;
        }

        template<typename T>
        bool RemoveComponent() {
            // Iterate over all of the components and try to dynamic cast them to be the type of 
            // component we are looking for. If the static cast is not null then we have found
            // the component to remove.
            for (int i = 0; i < m_Components.size(); i++) {
                T* existingComponent = dynamic_cast<T*>(m_Components.at(i));
                if (existingComponent != nullptr) {
                    m_Components.erase(m_Components.begin() + i);
                    delete existingComponent;
                    return true;
                }
            }
            return false;
        }

    };

}

#endif