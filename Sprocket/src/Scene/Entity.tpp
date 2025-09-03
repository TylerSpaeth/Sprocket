#ifndef ENTITY_TPP
#define ENTITY_TPP

#include "Entity.h"
#include "Components/QuadRendererComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/TileMapComponent.h"
#include "Components/SoundComponent.h"

namespace Sprocket {

    // This file is for explicit specialization of the Add/Get/Remove component functions of the 
    // Entity class. It allows the header to stay uncluttered while removing the need to include 
    // the cpp file.

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
                existingComponent->UpdateCameraTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
                delete existingComponent;
                return true;
            }
        }
        return false;
    }

    template<>
    inline bool Entity::AddComponent<SoundComponent>() {
        for (Component* component : m_Components) {
            SoundComponent* existingComponent = dynamic_cast<SoundComponent*>(component);
            if (existingComponent != nullptr) {
                return false;
            }
        }
        SoundComponent* sound = new SoundComponent();
        if (m_EventCallback != nullptr) {
            sound->m_EventCallback = m_EventCallback;
        }
        m_Components.push_back(sound);
        return true;
    }

    template<>
    inline bool Entity::RemoveComponent<SoundComponent>() {
        for (int i = 0; i < m_Components.size(); i++) {
            SoundComponent* existingComponent = dynamic_cast<SoundComponent*>(m_Components.at(i));
            if (existingComponent != nullptr) {
                if(existingComponent->m_EventCallback != nullptr) {
                    m_Components.erase(m_Components.begin() + i);
                    if (existingComponent->IsPlaying()) {
                        existingComponent->Stop();
                    }
                    delete existingComponent;
                    return true;
                }
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

    template<>
    inline TransformComponent* Entity::GetComponent<TransformComponent>() {
        return &m_Transform;
    }

}

#endif