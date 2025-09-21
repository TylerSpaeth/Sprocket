#include "ColliderComponent.h"

#include "Events/PhysicsEvent.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    float CircleColliderComponent::GetRadius() const {
        return m_Radius;
    }

    bool CircleColliderComponent::SetRadius(float radius) {
        if (radius < 0) {
            return false;
        }

        m_Radius = radius;

        if (m_EventCallback) {

            PhysicsUpdateEvent* event = new PhysicsUpdateEvent(m_PhysicsID, m_TranformComponent->Position(), m_Radius * m_TranformComponent->Scale().x);

            m_EventCallback(*event);

            delete event;

        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    CircleColliderComponent::CircleColliderComponent(TransformComponent& transformComponent) : ColliderComponent(transformComponent) {}

    void CircleColliderComponent::Register() {

        if (m_EventCallback) {

            PhysicsNewEvent* event = new PhysicsNewEvent(m_TranformComponent->Position(), m_Radius * m_TranformComponent->Scale().x);

            m_EventCallback(*event);

            m_PhysicsID = event->GetPhysicsID();

            delete event;

        }

    }

    void CircleColliderComponent::UpdateTransform() {

        if (m_EventCallback) {

            PhysicsUpdateEvent* event = new PhysicsUpdateEvent(m_PhysicsID, m_TranformComponent->Position(), m_Radius * m_TranformComponent->Scale().x);

            m_EventCallback(*event);

            delete event;
        }
    }

}