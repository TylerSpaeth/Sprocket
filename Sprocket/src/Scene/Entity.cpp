#include "Entity.h"

#include "Components/QuadRendererComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/TileMapComponent.h"
#include "Components/SoundComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/TextRendererComponent.h"

namespace Sprocket {

    Entity::Entity() : m_Transform([this]() {return GetParentGlobalTransform(); }) {
        InitializeAllowedComponents();
    }

    Entity::~Entity() {
        FreeAllowedComponents();
    }

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
                qr->RegisterEventCallback(m_EventCallback);
                qr->UpdateModelMatrix(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
                qr->UpdateQuadColor(qr->GetQuadColor());
            }
            else if (CameraComponent* camera = dynamic_cast<CameraComponent*>(component)) {
                camera->RegisterEventCallback(m_EventCallback);
                camera->UpdateCameraTransform(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
            }
            else if (ColliderComponent* collider = dynamic_cast<ColliderComponent*>(component)) {
                collider->RegisterEventCallback(m_EventCallback);
                collider->Register();
            }
            else if (TileMapComponent* tileMap = dynamic_cast<TileMapComponent*>(component)) {
                tileMap->RegisterEventCallback(m_EventCallback);
                tileMap->RegisterTileMap(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
            }
            else if (SoundComponent* sound = dynamic_cast<SoundComponent*>(component)) {
                sound->RegisterEventCallback(m_EventCallback);
                if (!sound->GetFilepath().empty() && sound->m_SoundID == -1) {
                    sound->SetFilepath(sound->GetFilepath());
                }
            }
            else if (AnimationComponent* animation = dynamic_cast<AnimationComponent*>(component)) {
                animation->RegisterEventCallback(m_EventCallback);
            }
            if (TextRendererComponent* tr = dynamic_cast<TextRendererComponent*>(component)) {
                tr->RegisterEventCallback(m_EventCallback);
                tr->UpdateModelMatrix(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
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
            if (AnimationComponent* animation = dynamic_cast<AnimationComponent*>(component)) {
                animation->m_QuadRenderer->RemoveRender();
                animation->m_EventCallback = nullptr;
                animation->m_QuadRenderer->m_EventCallback = nullptr;
            }
            if (TextRendererComponent* tr = dynamic_cast<TextRendererComponent*>(component)) {
                tr->RemoveRender();
                tr->m_EventCallback = nullptr;
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
                else if (AnimationComponent* animation = dynamic_cast<AnimationComponent*>(component)) {
                    animation->m_QuadRenderer->UpdateModelMatrix(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
                }
                else if (TextRendererComponent* tr = dynamic_cast<TextRendererComponent*>(component)) {
                    tr->UpdateModelMatrix(m_Transform.Position(), m_Transform.Rotation(), m_Transform.Scale());
                }
            }
            for (auto child : m_Children) {
                child->m_Transform.m_Modified = true;
            }

            m_Transform.m_Modified = false;
        }

        // Any per frame updates for components
        for (Component* component : m_Components) {
            if (AnimationComponent* animation = dynamic_cast<AnimationComponent*>(component)) {
                animation->UpdateAnimation(deltaTime);
            }
        }

        Update(deltaTime);

    }

    void Entity::InitializeAllowedComponents() {
        // Transform is set to 0 since that can never be added to an entity
        m_AllowedComponents.insert({typeid(TransformComponent), new unsigned int(0)});
        m_AllowedComponents.insert({typeid(CameraComponent), new unsigned int(1)});
        auto maximumColliders = new unsigned int(1);
        m_AllowedComponents.insert({typeid(ColliderComponent), maximumColliders});
        m_AllowedComponents.insert({typeid(BoxColliderComponent), maximumColliders});
        m_AllowedComponents.insert({typeid(CircleColliderComponent), maximumColliders});
        auto maximumRenderers = new unsigned int(1);
        m_AllowedComponents.insert({typeid(QuadRendererComponent), maximumRenderers});
        m_AllowedComponents.insert({typeid(AnimationComponent), maximumRenderers});
        m_AllowedComponents.insert({typeid(TextRendererComponent), maximumRenderers});
        m_AllowedComponents.insert({typeid(SoundComponent), new unsigned int(1)});
        m_AllowedComponents.insert({typeid(TileMapComponent), new unsigned int(1)});
    }

    void Entity::FreeAllowedComponents() {
        for(const auto& pair : m_AllowedComponents) {
            if(pair.second != nullptr) {
                delete pair.second;
            }
        }
        m_AllowedComponents.clear();
    }

}