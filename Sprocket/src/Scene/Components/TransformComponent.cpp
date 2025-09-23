#include "TransformComponent.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    TransformComponent::TransformComponent(std::function<TransformComponent()> callback) : m_GlobalTransformCallback(callback) {}

    glm::vec3& TransformComponent::LocalPosition() {
        m_Modified = true;
        return m_LocalPosition;
    }

    glm::vec3& TransformComponent::LocalRotation() {
        m_Modified = true;
        return m_LocalRotation;
    }

    glm::vec3& TransformComponent::LocalScale() {
        m_Modified = true;
        return m_LocalScale;
    }

    const glm::vec3 TransformComponent::Position() {
        return m_LocalPosition + m_GlobalTransformCallback().m_LocalPosition;
    }

    const glm::vec3 TransformComponent::Rotation() {
        return m_LocalRotation + m_GlobalTransformCallback().m_LocalRotation;
    }

    const glm::vec3 TransformComponent::Scale() {
        return m_LocalScale * m_GlobalTransformCallback().m_LocalScale;
    }

}