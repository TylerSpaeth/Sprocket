#ifndef ENTITY_TPP
#define ENTITY_TPP

#include "Entity.h"
#include "Components/QuadRendererComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/TileMapComponent.h"
#include "Components/SoundComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/TextRendererComponent.h"

namespace Sprocket {

    // This file is for explicit specialization of the Add/Get/Remove component functions of the 
    // Entity class. It allows the header to stay uncluttered while removing the need to include 
    // the cpp file.

    template<>
    inline const bool Entity::AddComponent<BoxColliderComponent>() {

        auto it = m_AllowedComponents.find(typeid(BoxColliderComponent));
        if (*it->second == 0) {
            Global::fileLogger.Warning("Max number of BoxColliderComponents on this object reached. Ignoring add.");
            return false;
        }

        auto boxCollider = std::shared_ptr<BoxColliderComponent>(
            new BoxColliderComponent(*m_Transform)
        );

        if (m_EventCallback != nullptr) {
            boxCollider->m_EventCallback = m_EventCallback;
            boxCollider->Register();
        }

        m_Components.push_back(boxCollider);

        (*it->second)--;

        return true;
    }

    template<>
    inline const bool Entity::AddComponent<CircleColliderComponent>() {

        auto it = m_AllowedComponents.find(typeid(CircleColliderComponent));
        if (*it->second == 0) {
            Global::fileLogger.Warning("Max number of CircleColliderComponents on this object reached. Ignoring add.");
            return false;
        }

        auto circleCollider = std::shared_ptr<CircleColliderComponent>(
            new CircleColliderComponent(*m_Transform)
        );

        if (m_EventCallback != nullptr) {
            circleCollider->m_EventCallback = m_EventCallback;
            circleCollider->Register();
        }

        m_Components.push_back(circleCollider);

        (*it->second)--;

        return true;
    }

    template<>
    inline std::shared_ptr<TransformComponent> Entity::GetComponent<TransformComponent>() {
        return m_Transform;
    }

}

#endif