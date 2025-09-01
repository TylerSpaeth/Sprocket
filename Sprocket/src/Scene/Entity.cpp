#include "Entity.h"

namespace Sprocket {

    Entity::Entity() : m_Transform([this]() {return GetParentGlobalTransform(); }) {}

    Entity::~Entity() {}

    TransformComponent Entity::GetParentGlobalTransform() {
        if (!m_Parent) return TransformComponent(nullptr);

        TransformComponent ret(nullptr);
        ret.m_LocalPosition = m_Parent->m_Transform.Position();
        ret.m_LocalRotation = m_Parent->m_Transform.Rotation();
        ret.m_LocalScale = m_Parent->m_Transform.Scale();
        return ret;
    }

    void Entity::OnEvent(Event& event) {}

    void Entity::OnActivate() {
        for (Component* component : m_Components) {
            // If this component is of they QuadRendererComponent, remove its event callback
            if (QuadRendererComponent* qr = dynamic_cast<QuadRendererComponent*>(component)) {
                qr->m_EventCallback = m_EventCallback;
                qr->RenderNew(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
                qr->UpdateQuadColor(qr->GetQuadColor());
            }
            else if (CameraComponent* camera = dynamic_cast<CameraComponent*>(component)) {
                camera->m_EventCallback = m_EventCallback;
                camera->UpdateCameraTransform(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
            }
            else if (ColliderComponent* collider = dynamic_cast<ColliderComponent*>(component)) {
                collider->m_EventCallback = m_EventCallback;
                collider->Register();
            }
            else if (TileMapComponent* tileMap = dynamic_cast<TileMapComponent*>(component)) {
                tileMap->m_EventCallback = m_EventCallback;
                tileMap->RegisterTileMap(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
            }
            else if (SoundComponent* sound = dynamic_cast<SoundComponent*>(component)) {
                sound->m_EventCallback = m_EventCallback;
                if (!sound->GetFilepath().empty() && sound->m_SoundID == -1) {
                    sound->SetFilepath(sound->GetFilepath());
                }
            }
        }
        Start();
    }

    void Entity::OnDeactivate() {

        for (Component* component : m_Components) {
            // If this component is of they QuadRendererComponent, remove its event callback
            if (QuadRendererComponent* qr = dynamic_cast<QuadRendererComponent*>(component)) {
                qr->RemoveRender();
                qr->m_EventCallback = nullptr;
            }
            else if (CameraComponent* camera = dynamic_cast<CameraComponent*>(component)) {
                camera->UpdateCameraTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
                camera->m_EventCallback = nullptr;
            }
            else if (ColliderComponent* collider = dynamic_cast<ColliderComponent*>(component)) {
                collider->Remove();
                collider->m_EventCallback = nullptr;
            }
            else if (TileMapComponent* tileMap = dynamic_cast<TileMapComponent*>(component)) {
                tileMap->DeleteTileMap();
                tileMap->m_EventCallback = nullptr;
            }
            else if (SoundComponent* sound = dynamic_cast<SoundComponent*>(component)) {
                sound->Stop();
                sound->m_EventCallback = nullptr;
            }
        }

        m_EventCallback = nullptr;
        End();
    }

    void Entity::OnUpdate(float deltaTime) {

        // Transform Updates
        if (m_Transform.m_Modified) {
            for (Component* component : m_Components) {
                if (QuadRendererComponent* qr = dynamic_cast<QuadRendererComponent*>(component)) {
                    qr->UpdateModelMatrix(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
                }
                else if (CameraComponent* camera = dynamic_cast<CameraComponent*>(component)) {
                    camera->UpdateCameraTransform(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
                }
                else if (ColliderComponent* collider = dynamic_cast<ColliderComponent*>(component)) {
                    collider->UpdateTransform();
                }
                else if (TileMapComponent* tileMap = dynamic_cast<TileMapComponent*>(component)) {
                    tileMap->UpdateTransform(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
                }
            }
        }

        Update(deltaTime);
    }

}