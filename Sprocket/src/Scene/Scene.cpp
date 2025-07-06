#include "Scene.h"

#include "Events/ApplicationEvent.h"

#include <algorithm>
#include <iostream>

namespace Sprocket {

  Scene::Scene() {
    // TODO
  }

  Scene::~Scene() {
    // TODO
  }

  void Scene::SubmitEntityToScene(Entity& entity) {
    // TODO validate entity before adding it to scene
    m_Entities.push_back(&entity);
    entity.m_EventCallback = m_EventCallback;
    if(m_EventCallback) {
      entity.OnActivate();
    }
  }

  bool Scene::RemoveEntityFromScene(Entity& entity) {
    std::vector<Entity*>::const_iterator position = std::find(m_Entities.cbegin(), m_Entities.cend(), &entity);
    if(position != m_Entities.cend()) {
      m_Entities.erase(position);
      return true;
    }
    return false;
  }

  void Scene::AssignEntityParent(Entity& child, Entity* parent) {

    if(!parent) {
      child.m_Parent = nullptr;
      return;
    }

    if(parent && std::find(parent->m_Children.cbegin(), parent->m_Children.cend(), &child) == parent->m_Children.cend()) {

      // Remove child from children of current parent
      Entity* currentParent = child.m_Parent;
      if(currentParent) {
        std::vector<Entity*>::const_iterator it = std::find(currentParent->m_Children.cbegin(), currentParent->m_Children.cend(), &child);

        currentParent->m_Children.erase(it);
      }

      // Assign as child of new parent
      parent->m_Children.push_back(&child);
      child.m_Parent = parent;
    }
    
  }

  void Scene::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
    m_EventCallback = eventCallback;
  }

  void Scene::OnEvent(Event& event) {
    switch(event.GetEventType()) {
      case EventType::APP_UPDATE:
        for(Entity* entity : m_Entities) {
          entity->OnEvent(event);
        }
        OnUpdate(((ApplicationUpdateEvent&)event).GetDeltaTime());
        break;
    }
    //TODO handle events
  }

  void Scene::OnActivate() {
    for(Entity* entity : m_Entities) {
      entity->m_EventCallback = m_EventCallback;
      entity->OnActivate();
    }
    // TODO
  }

  void Scene::OnDeactivate() {
    for(Entity* entity : m_Entities) {
      entity->OnDeactivate();
    }
    // TODO
  }

  void Scene::OnUpdate(float deltaTime) {
    for(Entity* entity : m_Entities) {
      entity->OnUpdate(deltaTime);
    }
    // TODO
  }

  

}