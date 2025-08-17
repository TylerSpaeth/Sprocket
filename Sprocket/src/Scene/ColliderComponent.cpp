#include "ColliderComponent.h"

namespace Sprocket {

    bool ColliderComponent::CollidesWith(ColliderComponent& colliderComponent) {

        bool result = false;

        if (m_EventCallback && m_PhysicsID != -1 && colliderComponent.GetPhysicsID() != -1) {

            PhysicsCollisionCheckEvent* event = new PhysicsCollisionCheckEvent(m_PhysicsID, colliderComponent.GetPhysicsID());

            m_EventCallback(*event);

            result = event->Collides();

            free(event);

        }

        return result;

    }

    void ColliderComponent::Remove() {

        if (m_EventCallback) {

            PhysicsDeleteEvent* event = new PhysicsDeleteEvent(m_PhysicsID);

            m_EventCallback(*event);

            free(event);

        }

    }

    bool ColliderComponent::CollidesWithAnything() {

        bool result = false;

        if (m_EventCallback) {
            PhysicsCollisionCheckGenericEvent* event = new PhysicsCollisionCheckGenericEvent(m_PhysicsID);
            m_EventCallback(*event);
            result = event->Collides();
            free(event);
        }

        return result;

    }

}