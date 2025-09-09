#ifndef ENTITY_TPP
#define ENTITY_TPP

#include "Entity.h"
#include "Components/QuadRendererComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/TileMapComponent.h"
#include "Components/SoundComponent.h"
#include "Components/AnimationComponent.h"

namespace Sprocket {

    // This file is for explicit specialization of the Add/Get/Remove component functions of the 
    // Entity class. It allows the header to stay uncluttered while removing the need to include 
    // the cpp file.

    /*template<>
    inline bool Entity::AddComponent<QuadRendererComponent>() {

        auto it = m_AllowedComponents.find(typeid(QuadRendererComponent));
        if (*it->second == 0) {
            Global::fileLogger.Warning("Max number of QuadRendererComponents on this object reached. Ignoring add.");
            return false;
        }

        QuadRendererComponent* qr = new QuadRendererComponent();

        if (m_EventCallback != nullptr) {
            qr->m_EventCallback = m_EventCallback;
            qr->RenderNew(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
        }

        m_Components.push_back(qr);

        (*it->second)--;

        return true;
    }

    template<>
    inline bool Entity::AddComponent<AnimationComponent>() {

        auto it = m_AllowedComponents.find(typeid(AnimationComponent));
        if (*it->second == 0) {
            Global::fileLogger.Warning("Max number of AnimationComponents on this object reached. Ignoring add.");
            return false;
        }

        AnimationComponent* component = new AnimationComponent();

        if (m_EventCallback != nullptr) {
            component->m_EventCallback = m_EventCallback;
            component->Register();
            component->m_QuadRenderer->RenderNew(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
        }

        m_Components.push_back(component);

        (*it->second)--;

        return true;
    }*/

    template<>
    inline bool Entity::AddComponent<BoxColliderComponent>() {

        auto it = m_AllowedComponents.find(typeid(BoxColliderComponent));
        if (*it->second == 0) {
            Global::fileLogger.Warning("Max number of BoxColliderComponents on this object reached. Ignoring add.");
            return false;
        }

        BoxColliderComponent* boxCollider = new BoxColliderComponent(m_Transform);

        if (m_EventCallback != nullptr) {
            boxCollider->m_EventCallback = m_EventCallback;
            boxCollider->Register();
        }

        m_Components.push_back(boxCollider);

        (*it->second)--;

        return true;
    }

    template<>
    inline bool Entity::AddComponent<CircleColliderComponent>() {

        auto it = m_AllowedComponents.find(typeid(CircleColliderComponent));
        if (*it->second == 0) {
            Global::fileLogger.Warning("Max number of CircleColliderComponents on this object reached. Ignoring add.");
            return false;
        }

        CircleColliderComponent* circleCollider = new CircleColliderComponent(m_Transform);

        if (m_EventCallback != nullptr) {
            circleCollider->m_EventCallback = m_EventCallback;
            circleCollider->Register();
        }

        m_Components.push_back(circleCollider);

        (*it->second)--;

        return true;
    }

    template<>
    inline TransformComponent* Entity::GetComponent<TransformComponent>() {
        return &m_Transform;
    }

}

#endif