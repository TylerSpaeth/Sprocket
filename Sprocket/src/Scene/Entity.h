#ifndef ENTITY_H
#define ENTITY_H

#include "Core/Macros.h"

#include "Events/Event.h"

#include "Components/Component.h"
#include "Components/TransformComponent.h"

#include "Core/Sprocket.pch"
#include "Core/Global.h"

namespace Sprocket {

    /// @brief A sort of container for for Components with some extra functionality
    class SPROCKET_API Entity {

        friend class Scene;

    private:

        std::function<void(Event&)> m_EventCallback;

        Entity* m_Parent = nullptr;

        std::vector<Entity*> m_Children;

        // A map, mapping difference component types to how many more of that component
        // can be added to this entity.
        std::unordered_map<std::type_index, unsigned int*> m_AllowedComponents;

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

        /// @brief Initialize the m_AllowedComponents map with component types and how many more
        /// of that type can be added to this entity.
        void InitializeAllowedComponents();

        /// @brief Frees the data in the m_AllowedComponents map
        void FreeAllowedComponents();

    public:

        Entity();
        ~Entity();

        virtual void Start() {}
        virtual void Update(float deltaTime) {}
        virtual void End() {}

        // Only one of each component type is allowed on a single Entity
        template<typename T>
        bool AddComponent() {

            // Check if this component is allowed to have more of its type added
            std::type_index type = typeid(T);
            auto it = m_AllowedComponents.find(type);
            if (it == m_AllowedComponents.end()) {
                Global::fileLogger.Warning("Invalid component type, ignoring add.");
                return false;
            }
            if (*(it->second) == 0) {
                Global::fileLogger.Warning(std::format("Max number of {} on this entity reached. Ignoring add.", typeid(T).name()));
                return false;
            }

            // Decrement the number of components that can now be added of this type
            (*it->second)--;
            T* newComponent = new T();
            m_Components.push_back(newComponent);

            // If this is an event driven component and we have an eventcallback, register
                        // the event callback on the component
            if (std::is_base_of<EventDrivenComponent, T>::value) {
                if (m_EventCallback) {
                    ((EventDrivenComponent*)newComponent)->RegisterEventCallback(m_EventCallback);
                }
            }

            return true;
        }

        template<typename T>
        T* GetComponent() {

            // Verify that this is a valid component type
            std::type_index type = typeid(T);
            auto it = m_AllowedComponents.find(type);
            if (it == m_AllowedComponents.end()) {
                Global::fileLogger.Warning("Invalid component type, ignoring get.");
                return nullptr;
            }

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

            // Verify that this is a valid component type
            std::type_index type = typeid(T);
            auto it = m_AllowedComponents.find(type);
            if (it == m_AllowedComponents.end()) {
                Global::fileLogger.Warning("Invalid file type, ignoring remove.");
                return false;
            }

            // Iterate over all of the components and try to dynamic cast them to be the type of 
            // component we are looking for. If the static cast is not null then we have found
            // the component to remove.
            for (int i = 0; i < m_Components.size(); i++) {
                T* existingComponent = dynamic_cast<T*>(m_Components.at(i));
                if (existingComponent != nullptr) {
                    // Since we are removing this component we can allow another to be created
                    (*it->second)++;
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