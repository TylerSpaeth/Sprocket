#include "Scene.h"

#include "Events/ApplicationEvent.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    const bool Scene::SubmitEntityToScene(std::shared_ptr<Entity> entity) {

        // If this entity is already in the scene
        std::vector<std::shared_ptr<Entity>>::const_iterator position = std::find_if(m_Entities.cbegin(), m_Entities.cend(), 
            [&](const std::shared_ptr<Entity>& other) {return other.get() == entity.get();});
        if (position != m_Entities.cend()) {
            return false;
        }

        m_Entities.push_back(entity);
        entity->m_EventCallback = m_EventCallback;
        if (m_EventCallback) {
            entity->OnActivate();
        }

        return true;
    }

    const bool Scene::RemoveEntityFromScene(std::weak_ptr<Entity> entity) {
        std::vector<std::shared_ptr<Entity>>::const_iterator position = std::find_if(m_Entities.cbegin(), m_Entities.cend(),
            [&](const std::shared_ptr<Entity>& other) {return other.get() == entity.lock().get(); });
        if (position != m_Entities.cend()) {
            (*position)->OnDeactivate(); // Deactivate the entity to remove it from the scene
            m_Entities.erase(position);
            return true;
        }
        return false;
    }

    const bool Scene::AssignEntityParent(std::shared_ptr<Entity> child, std::shared_ptr<Entity> parent) {

        // Verify the child is in the scene
        std::vector<std::shared_ptr<Entity>>::const_iterator childPosition = std::find_if(m_Entities.cbegin(), m_Entities.cend(),
            [&](const std::shared_ptr<Entity>& other) {return other.get() == child.get(); });
        if (childPosition == m_Entities.cend()) {
            return false;
        }

        // If the child should have no parent
        if (!parent) {
            child->m_Parent.reset();
            return true;
        }

        // Verify the parent is in the scene
        std::vector<std::shared_ptr<Entity>>::const_iterator parentPosition = std::find_if(m_Entities.cbegin(), m_Entities.cend(),
            [&](const std::shared_ptr<Entity>& other) {return other.get() == parent.get(); });
        if (parentPosition == m_Entities.cend()) {
            return false;
        }

        // Check to see if the child is already a child of this parent
        if (std::find_if(parent->m_Children.cbegin(), parent->m_Children.cend(),
            [&](const std::weak_ptr<Entity>& other) {return other.lock().get() == child.get(); }) == parent->m_Children.cend()) {

            // Remove child from children of current parent
            auto currentParent = child.get()->m_Parent;
            if (auto lockedParent = currentParent.lock()) {
                std::vector<std::weak_ptr<Entity>>::const_iterator it = std::find_if(parent->m_Children.cbegin(), parent->m_Children.cend(),
                    [&](const std::weak_ptr<Entity>& other) {return other.lock().get() == child.get(); });

                lockedParent->m_Children.erase(it);
            }

            // Assign as child of new parent
            parent->m_Children.push_back(child);
            child->m_Parent = parent;

        }

        return true;;

    }

    const void Scene::OnEvent(Event& event) {
        switch (event.GetEventType()) {
        case EventType::APP_UPDATE:
            for (auto& entity : m_Entities) {
                entity->OnEvent(event);
            }
            OnUpdate(((ApplicationUpdateEvent&)event).GetDeltaTime());
            break;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Scene::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
        m_EventCallback = eventCallback;
    }

    void Scene::OnActivate() {
        for (auto& entity : m_Entities) {
            entity->m_EventCallback = m_EventCallback;
            entity->OnActivate();
        }
    }

    void Scene::OnDeactivate() {
        for (auto& entity : m_Entities) {
            entity->OnDeactivate();
            entity->m_EventCallback = nullptr;
        }
    }

    void Scene::OnUpdate(float deltaTime) {
        for (int i = 0; i < m_Entities.size(); i++) {
            m_Entities.at(i)->OnUpdate(deltaTime);
        }
    }



}