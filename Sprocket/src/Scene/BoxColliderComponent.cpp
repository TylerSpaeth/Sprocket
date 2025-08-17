#include "ColliderComponent.h"

namespace Sprocket {

    glm::vec2 BoxColliderComponent::GetSize() const {
        return m_Size;
    }

    bool BoxColliderComponent::SetSize(const glm::vec2 size) {

        if (size.x < 0 || size.y < 0) {
            return false;
        }

        m_Size = size;

        if (m_EventCallback) {

            glm::vec2 scale = m_TranformComponent->Scale();

            PhysicsUpdateEvent* event = new PhysicsUpdateEvent(m_PhysicsID, m_TranformComponent->Position(), { scale.x * m_Size.x, scale.y * m_Size.y }, m_TranformComponent->Rotation().z);

            m_EventCallback(*event);

            delete event;

        }

        return true;

    }

    void BoxColliderComponent::Register() {

        if (m_EventCallback) {

            glm::vec2 scale = m_TranformComponent->Scale();

            PhysicsNewEvent* event = new PhysicsNewEvent(m_TranformComponent->Position(), { scale.x * m_Size.x, scale.y * m_Size.y }, m_TranformComponent->Rotation().z);

            m_EventCallback(*event);

            m_PhysicsID = event->GetPhysicsID();

            delete event;

        }

    }

    void BoxColliderComponent::UpdateTransform() {

        if (m_EventCallback) {

            glm::vec2 scale = m_TranformComponent->Scale();

            PhysicsUpdateEvent* event = new PhysicsUpdateEvent(m_PhysicsID, m_TranformComponent->Position(), { scale.x * m_Size.x, scale.y * m_Size.y }, m_TranformComponent->Rotation().z);

            m_EventCallback(*event);

            delete event;

        }

    }

}