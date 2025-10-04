#include "ColliderComponent.h"

#include "Events/PhysicsEvent.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    int ColliderComponent::GetPhysicsID() const {
        return m_PhysicsID;
    }

    const bool ColliderComponent::CollidesWith(ColliderComponent& colliderComponent) {

        bool result = false;

        if (m_EventCallback && m_PhysicsID != -1 && colliderComponent.GetPhysicsID() != -1) {

            PhysicsCollisionCheckEvent* event = new PhysicsCollisionCheckEvent(m_PhysicsID, colliderComponent.GetPhysicsID());

            m_EventCallback(*event);

            result = event->Collides();

            delete event;

        }

        return result;

    }

    const bool ColliderComponent::CollidesWithAnything() {

        bool result = false;

        if (m_EventCallback) {
            PhysicsCollisionCheckGenericEvent* event = new PhysicsCollisionCheckGenericEvent(m_PhysicsID);
            m_EventCallback(*event);
            result = event->Collides();
            delete event;
        }

        return result;

    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////PROTECTED///////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ColliderComponent::ColliderComponent(TransformComponent& transformComponent) : m_TranformComponent(&transformComponent) {}

    ColliderComponent::~ColliderComponent() {
        if (m_EventCallback && m_PhysicsID != -1) {
            Remove();
        }
    }

    void ColliderComponent::Remove() {

        if (m_EventCallback) {

            PhysicsDeleteEvent* event = new PhysicsDeleteEvent(m_PhysicsID);

            m_EventCallback(*event);

            delete event;

        }

    }

    void ColliderComponent::OnActivate(OnActivateParams& onActivateParams) {
        RegisterEventCallback(onActivateParams.eventCallback);
        Register();
    }

    void ColliderComponent::OnDeactivate(OnDeactivateParams& onDeactivateParams) {
        Remove();
        m_EventCallback = nullptr;
    }

    void ColliderComponent::OnUpdate(OnUpdateParams& onUpdateParams) {
        UpdateTransform();
    }
}