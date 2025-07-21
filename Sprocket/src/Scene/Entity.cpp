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
      if(QuadRendererComponent* qr = dynamic_cast<QuadRendererComponent*>(component)) {
        qr->m_EventCallback = m_EventCallback;
        qr->RenderNew(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
        qr->UpdateQuadColor(qr->GetQuadColor());
      }
      else if(CameraComponent* camera = dynamic_cast<CameraComponent*>(component)) {
        camera->m_EventCallback = m_EventCallback;
        camera->UpdateCameraPosition(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
      }
      else if(ColliderComponent* collider = dynamic_cast<ColliderComponent*>(component)) {
        collider->m_EventCallback = m_EventCallback;
        collider->Register();
      }
      else if(TileMapComponent* tileMap = dynamic_cast<TileMapComponent*>(component)) {
        tileMap->m_EventCallback = m_EventCallback;
        tileMap->RegisterTileMap(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
      }
    }
    Start();
  }

  void Entity::OnDeactivate() {

    for(Component* component : m_Components) {
      // If this component is of they QuadRendererComponent, remove its event callback
      if(QuadRendererComponent* qr = dynamic_cast<QuadRendererComponent*>(component)) {
        qr->RemoveRender();
        qr->m_EventCallback = nullptr;
      }
      else if(CameraComponent* camera = dynamic_cast<CameraComponent*>(component)) {
        camera->UpdateCameraPosition(glm::vec3(0), glm::vec3(0), glm::vec3(1));
        camera->m_EventCallback = nullptr;
      }
      else if(ColliderComponent* collider = dynamic_cast<ColliderComponent*>(component)) {
        collider->Remove();
        collider->m_EventCallback = nullptr;
      }
      else if(TileMapComponent* tileMap = dynamic_cast<TileMapComponent*>(component)) {
        tileMap->DeleteTileMap();
        tileMap->m_EventCallback = nullptr;
      }
    }

    m_EventCallback = nullptr;
    End();
  }

  // TODO this function needs to be fixed. It is highly inefficient to doing all of these updates 
  // every frame if the transform is not changing at all. It would be much better to have a flag
  // for if the transform has been updated any only perform transform updates if it is set.
  void Entity::OnUpdate(float deltaTime) {
    
    for(Component* component : m_Components) {
      if(QuadRendererComponent* qr = dynamic_cast<QuadRendererComponent*>(component)) {
        qr->UpdateModelMatrix(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
      }
      else if(CameraComponent* camera = dynamic_cast<CameraComponent*>(component)) {
        camera->UpdateCameraPosition(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
      }
      else if(ColliderComponent* collider = dynamic_cast<ColliderComponent*>(component)) {
        collider->UpdateTransform();
      }
      else if(TileMapComponent* tileMap = dynamic_cast<TileMapComponent*>(component)) {
        tileMap->UpdateTransform(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
      }
    }

    Update(deltaTime);
  }

}