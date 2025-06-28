#include "Entity.h"

#include <iostream>

namespace Sprocket {

  Entity::Entity() : m_Transform([this]() {return GetParentGlobalTransform();}) {
    // TODO
  }

  Entity::~Entity() {
    // TODO
  }

  TransformComponent Entity::GetParentGlobalTransform() {
    if(!m_Parent) return TransformComponent(nullptr);

    TransformComponent ret(nullptr);
    ret.m_LocalPosition = m_Parent->m_Transform.Position();
    ret.m_LocalRotation = m_Parent->m_Transform.Rotation();
    ret.m_LocalScale = m_Parent->m_Transform.Scale();
    return ret;
  }

  void Entity::OnEvent(Event& event) {
    // TODO
  }

  void Entity::OnActivate() {
    Start();
    // TODO
  }

  void Entity::OnDeactivate() {
    End();
    // TODO
  }

  void Entity::OnUpdate(float deltaTime) {
    Update(deltaTime);
    // TODO
  }

}