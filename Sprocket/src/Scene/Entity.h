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

    protected:

        std::shared_ptr<Entity> m_Self;

    private: 

        std::function<void(Event&)> m_EventCallback;

        std::weak_ptr<Entity> m_Parent;

        std::vector<std::weak_ptr<Entity>> m_Children;

        // A map, mapping difference component types to how many more of that component
        // can be added to this entity.
        std::unordered_map<std::type_index, unsigned int*> m_AllowedComponents;

        std::vector<std::shared_ptr<Component>> m_Components;

        std::shared_ptr<TransformComponent> m_Transform;

    public:

        Entity();
        ~Entity();

        // Only one of each component type is allowed on a single Entity
        template<typename T>
        const bool AddComponent() {

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
            auto newComponent = std::make_shared<T>();
            //T* newComponent = new T();
            m_Components.push_back(newComponent);

            // If this is an event driven component and we have an eventcallback, register
                        // the event callback on the component
            if (std::is_base_of<EventDrivenComponent, T>::value) {
                if (m_EventCallback) {
                    ((std::shared_ptr<EventDrivenComponent>)newComponent)->RegisterEventCallback(m_EventCallback);
                }
            }

            return true;
        }

        template<typename T>
        std::shared_ptr<T> GetComponent() {

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
            for (auto component : m_Components) {
                auto existingComponent = dynamic_pointer_cast<T>(component);
                if (existingComponent != nullptr) {
                    return existingComponent;
                }
            }
            return nullptr;
        }

        template<typename T>
        const bool RemoveComponent() {

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
                auto existingComponent = dynamic_pointer_cast<T>(m_Components.at(i));
                if (existingComponent != nullptr) {
                    // Since we are removing this component we can allow another to be created
                    (*it->second)++;
                    m_Components.erase(m_Components.begin() + i);
                    return true;
                }
            }
            return false;
        }

    protected:

        virtual void Start();
        virtual void Update(float deltaTime);
        virtual void End();

    private:

        // Returns a transform representing the global of this entities parent.. The local values
        // are all the matter on this component. Global values are invalid.
        const TransformComponent GetParentGlobalTransform();

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

    };

}

#endif