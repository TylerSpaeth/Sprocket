#include "TransformComponent.h"

namespace Sprocket {

  glm::vec3& TransformComponent::LocalPosition() {
    return m_LocalPosition;
  }

  glm::vec3& TransformComponent::LocalRotation() {
    return m_LocalRotation;
  }

  glm::vec3& TransformComponent::LocalScale() {
    return m_LocalScale;
  }

  glm::vec3 TransformComponent::Position() {
    return m_LocalPosition + m_GlobalTransformCallback().m_LocalPosition;
  }

  glm::vec3 TransformComponent::Rotation() {
    return m_LocalRotation + m_GlobalTransformCallback().m_LocalRotation;
  }

  glm::vec3 TransformComponent::Scale() {
    return m_LocalScale * m_GlobalTransformCallback().m_LocalScale;
  }

}