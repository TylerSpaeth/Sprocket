#include "Entity.h"
#include "QuadRendererComponent.h"

#include <iostream>

namespace Sprocket {

  Entity::Entity() : m_Transform([this]() {return GetParentGlobalTransform();}) {}

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
    for(Component* component : m_Components) {
      // If this component is of they QuadRendererComponent, remove its event callback
      if(QuadRendererComponent* qr = static_cast<QuadRendererComponent*>(component)) {
        qr->m_EventCallback = m_EventCallback;
        qr->RenderNew(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
      }
    }
    Start();
  }

  void Entity::OnDeactivate() {

    for(Component* component : m_Components) {
      // If this component is of they QuadRendererComponent, remove its event callback
      if(QuadRendererComponent* qr = static_cast<QuadRendererComponent*>(component)) {
        qr->RemoveRender();
        qr->m_EventCallback = nullptr;
      }
    }

    m_EventCallback = nullptr;
    End();
  }

  void Entity::OnUpdate(float deltaTime) {
    
    for(Component* component : m_Components) {
      if(QuadRendererComponent* qr = static_cast<QuadRendererComponent*>(component)) {
        qr->UpdateModelMatrix(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
      }
    }

    Update(deltaTime);
  }

}