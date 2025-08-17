#include "Scene.h"

#include "Events/ApplicationEvent.h"

namespace Sprocket {

    Scene::Scene() {}

    Scene::~Scene() {}

    bool Scene::SubmitEntityToScene(Entity& entity) {

        // If this entity is already in the scene
        std::vector<Entity*>::const_iterator position = std::find(m_Entities.cbegin(), m_Entities.cend(), &entity);
        if (position != m_Entities.cend()) {
            return false;
        }

        m_Entities.push_back(&entity);
        entity.m_EventCallback = m_EventCallback;
        if (m_EventCallback) {
            entity.OnActivate();
        }

        return true;
    }

    bool Scene::RemoveEntityFromScene(Entity& entity) {
        std::vector<Entity*>::const_iterator position = std::find(m_Entities.cbegin(), m_Entities.cend(), &entity);
        if (position != m_Entities.cend()) {
            m_Entities.erase(position);
            return true;
        }
        return false;
    }

    bool Scene::AssignEntityParent(Entity& child, Entity* parent) {

        // Verify the child is in the scene
        std::vector<Entity*>::const_iterator childPosition = std::find(m_Entities.cbegin(), m_Entities.cend(), &child);
        if (childPosition == m_Entities.cend()) {
            return false;
        }

        // If the child should have no parent
        if (!parent) {
            child.m_Parent = nullptr;
            return true;
        }

        // Verify the parent is in the scene
        std::vector<Entity*>::const_iterator parentPosition = std::find(m_Entities.cbegin(), m_Entities.cend(), parent);
        if (parentPosition == m_Entities.cend()) {
            return false;
        }

        // Check to see if the child is already a child of this parent
        if (std::find(parent->m_Children.cbegin(), parent->m_Children.cend(), &child) == parent->m_Children.cend()) {

            // Remove child from children of current parent
            Entity* currentParent = child.m_Parent;
            if (currentParent) {
                std::vector<Entity*>::const_iterator it = std::find(currentParent->m_Children.cbegin(), currentParent->m_Children.cend(), &child);

                currentParent->m_Children.erase(it);
            }

            // Assign as child of new parent
            parent->m_Children.push_back(&child);
            child.m_Parent = parent;

        }

        return true;;

    }

    void Scene::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
        m_EventCallback = eventCallback;
    }

    void Scene::OnEvent(Event& event) {
        switch (event.GetEventType()) {
        case EventType::APP_UPDATE:
            for (Entity* entity : m_Entities) {
                entity->OnEvent(event);
            }
            OnUpdate(((ApplicationUpdateEvent&)event).GetDeltaTime());
            break;
        }
    }

    void Scene::OnActivate() {
        for (Entity* entity : m_Entities) {
            entity->m_EventCallback = m_EventCallback;
            entity->OnActivate();
        }
    }

    void Scene::OnDeactivate() {
        for (Entity* entity : m_Entities) {
            entity->OnDeactivate();
        }
    }

    void Scene::OnUpdate(float deltaTime) {
        for (Entity* entity : m_Entities) {
            entity->OnUpdate(deltaTime);
        }
    }



}