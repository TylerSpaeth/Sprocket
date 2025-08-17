#ifndef ENTITY_H
#define ENTITY_H

#include "Core/Macros.h"

#include "Events/Event.h"

#include "Component.h"
#include "TransformComponent.h"
#include "QuadRendererComponent.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "TileMapComponent.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

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

    // Override AddComponent and RemoveComponent for components that have special behavior

    template<>
    inline bool Entity::AddComponent<QuadRendererComponent>() {
        for (Component* component : m_Components) {
            QuadRendererComponent* existingComponent = dynamic_cast<QuadRendererComponent*>(component);
            if (existingComponent != nullptr) {
                return false;
            }
        }

        QuadRendererComponent* qr = new QuadRendererComponent();

        if (m_EventCallback != nullptr) {
            qr->m_EventCallback = m_EventCallback;
            qr->RenderNew(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
        }

        m_Components.push_back(qr);
        return true;
    }

    template<>
    inline bool Entity::RemoveComponent<QuadRendererComponent>() {
        for (int i = 0; i < m_Components.size(); i++) {
            QuadRendererComponent* existingComponent = dynamic_cast<QuadRendererComponent*>(m_Components.at(i));
            if (existingComponent != nullptr) {
                m_Components.erase(m_Components.begin() + i);
                existingComponent->RemoveRender();
                delete existingComponent;
                return true;
            }
        }
        return false;
    }

    template<>
    inline bool Entity::AddComponent<CameraComponent>() {
        for (Component* component : m_Components) {
            CameraComponent* existingComponent = dynamic_cast<CameraComponent*>(component);
            if (existingComponent != nullptr) {
                return false;
            }
        }

        CameraComponent* camera = new CameraComponent();

        if (m_EventCallback != nullptr) {
            camera->m_EventCallback = m_EventCallback;
        }

        m_Components.push_back(camera);
        return true;
    }

    template<>
    inline bool Entity::RemoveComponent<CameraComponent>() {
        for (int i = 0; i < m_Components.size(); i++) {
            CameraComponent* existingComponent = dynamic_cast<CameraComponent*>(m_Components.at(i));
            if (existingComponent != nullptr) {
                m_Components.erase(m_Components.begin() + i);
                existingComponent->UpdateCameraPosition(glm::vec3(0), glm::vec3(0), glm::vec3(1));
                delete existingComponent;
                return true;
            }
        }
        return false;
    }

    template<>
    inline bool Entity::AddComponent<BoxColliderComponent>() {

        for (Component* component : m_Components) {
            CircleColliderComponent* existingCircleCollider = dynamic_cast<CircleColliderComponent*>(component);
            BoxColliderComponent* existingBoxCollider = dynamic_cast<BoxColliderComponent*>(component);
            if (existingCircleCollider != nullptr || existingBoxCollider != nullptr) {
                return false;
            }
        }

        BoxColliderComponent* boxCollider = new BoxColliderComponent(m_Transform);

        if (m_EventCallback != nullptr) {
            boxCollider->m_EventCallback = m_EventCallback;
            boxCollider->Register();
        }

        m_Components.push_back(boxCollider);
        return true;
    }

    template<>
    inline bool Entity::RemoveComponent<BoxColliderComponent>() {
        for (int i = 0; i < m_Components.size(); i++) {
            BoxColliderComponent* existingComponent = dynamic_cast<BoxColliderComponent*>(m_Components.at(i));
            if (existingComponent != nullptr) {
                m_Components.erase(m_Components.begin() + i);
                existingComponent->Remove();
                delete existingComponent;
                return true;
            }
        }
        return false;
    }

    template<>
    inline bool Entity::AddComponent<CircleColliderComponent>() {
        for (Component* component : m_Components) {
            CircleColliderComponent* existingCircleCollider = dynamic_cast<CircleColliderComponent*>(component);
            BoxColliderComponent* existingBoxCollider = dynamic_cast<BoxColliderComponent*>(component);
            if (existingCircleCollider || existingBoxCollider) {
                return false;
            }
        }

        CircleColliderComponent* circleCollider = new CircleColliderComponent(m_Transform);

        if (m_EventCallback != nullptr) {
            circleCollider->m_EventCallback = m_EventCallback;
            circleCollider->Register();
        }

        m_Components.push_back(circleCollider);
        return true;
    }

    template<>
    inline bool Entity::RemoveComponent<CircleColliderComponent>() {
        for (int i = 0; i < m_Components.size(); i++) {
            CircleColliderComponent* existingComponent = dynamic_cast<CircleColliderComponent*>(m_Components.at(i));
            if (existingComponent != nullptr) {
                m_Components.erase(m_Components.begin() + i);
                existingComponent->Remove();
                delete existingComponent;
                return true;
            }
        }
        return false;
    }

    template<>
    inline bool Entity::AddComponent<TileMapComponent>() {
        for (Component* component : m_Components) {
            TileMapComponent* existingComponent = dynamic_cast<TileMapComponent*>(component);
            if (existingComponent) {
                return false;
            }
        }

        TileMapComponent* tileMap = new TileMapComponent();

        if (m_EventCallback != nullptr) {
            tileMap->m_EventCallback = m_EventCallback;
        }

        m_Components.push_back(tileMap);
        return true;
    }

    template<>
    inline bool Entity::RemoveComponent<TileMapComponent>() {
        for (int i = 0; i < m_Components.size(); i++) {
            TileMapComponent* existingComponent = dynamic_cast<TileMapComponent*>(m_Components.at(i));
            if (existingComponent != nullptr) {
                m_Components.erase(m_Components.begin() + i);
                existingComponent->DeleteTileMap();
                delete existingComponent;
                return true;
            }
        }
        return false;
    }

    // Override GetComponent for components not stored in m_Components

    template<>
    inline TransformComponent* Entity::GetComponent<TransformComponent>() {
        return &m_Transform;
    }

}

#endif