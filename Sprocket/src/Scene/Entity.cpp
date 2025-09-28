#include "Entity.h"

#include "Components/QuadRendererComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/TileMapComponent.h"
#include "Components/SoundComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/TextRendererComponent.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    Entity::Entity() {
        auto parentGlobalTransform = GetParentGlobalTransform();
        m_Transform = std::make_shared<TransformComponent>([this]() {return GetParentGlobalTransform(); });
        InitializeAllowedComponents();
    }

    Entity::~Entity() {
        FreeAllowedComponents();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////PROTECTED///////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Entity::Start() {}
    void Entity::Update(float deltaTime) {}
    void Entity::End() {}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    const TransformComponent Entity::GetParentGlobalTransform() {
        if (!m_Parent) return TransformComponent(nullptr);

        TransformComponent ret(nullptr);
        ret.m_LocalPosition = m_Parent->m_Transform.get()->Position();
        ret.m_LocalRotation = m_Parent->m_Transform.get()->Rotation();
        ret.m_LocalScale = m_Parent->m_Transform.get()->Scale();
        return ret;
    }

    void Entity::OnEvent(Event& event) {}

    void Entity::OnActivate() {
        for (auto component : m_Components) {
            // If this component is of they QuadRendererComponent, remove its event callback
            if (auto qr = dynamic_pointer_cast<QuadRendererComponent>(component)) {
                qr->RegisterEventCallback(m_EventCallback);
                qr->UpdateModelMatrix(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
                qr->SetQuadColor(qr->GetQuadColor());
            }
            else if (auto camera = dynamic_pointer_cast<CameraComponent>(component)) {
                camera->RegisterEventCallback(m_EventCallback);
                camera->UpdateCameraTransform(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
            }
            else if (auto collider = dynamic_pointer_cast<ColliderComponent>(component)) {
                collider->RegisterEventCallback(m_EventCallback);
                collider->Register();
            }
            else if (auto tileMap = dynamic_pointer_cast<TileMapComponent>(component)) {
                tileMap->RegisterEventCallback(m_EventCallback);
                tileMap->RegisterTileMap(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
            }
            else if (auto sound = dynamic_pointer_cast<SoundComponent>(component)) {
                sound->RegisterEventCallback(m_EventCallback);
                if (!sound->GetFilepath().empty() && sound->m_SoundID == -1) {
                    sound->SetFilepath(sound->GetFilepath());
                }
            }
            else if (auto animation = dynamic_pointer_cast<AnimationComponent>(component)) {
                animation->RegisterEventCallback(m_EventCallback);
                animation->m_QuadRenderer->UpdateModelMatrix(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
            }
            if (auto tr = dynamic_pointer_cast<TextRendererComponent>(component)) {
                tr->RegisterEventCallback(m_EventCallback);
                tr->UpdateModelMatrix(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
            }
        }
        Start();
    }

    void Entity::OnDeactivate() {

        for (auto component : m_Components) {
            // If this component is of they QuadRendererComponent, remove its event callback
            if (auto qr = dynamic_pointer_cast<QuadRendererComponent>(component)) {
                qr->RemoveRender();
                qr->m_EventCallback = nullptr;
            }
            else if (auto camera = dynamic_pointer_cast<CameraComponent>(component)) {
                camera->UpdateCameraTransform(glm::vec3(0), glm::vec3(0), glm::vec3(1));
                camera->m_EventCallback = nullptr;
            }
            else if (auto collider = dynamic_pointer_cast<ColliderComponent>(component)) {
                collider->Remove();
                collider->m_EventCallback = nullptr;
            }
            else if (auto tileMap = dynamic_pointer_cast<TileMapComponent>(component)) {
                tileMap->DeleteTileMap();
                tileMap->m_EventCallback = nullptr;
            }
            else if (auto sound = dynamic_pointer_cast<SoundComponent>(component)) {
                sound->Stop();
                sound->m_EventCallback = nullptr;
            }
            if (auto animation = dynamic_pointer_cast<AnimationComponent>(component)) {
                animation->m_QuadRenderer->RemoveRender();
                animation->m_EventCallback = nullptr;
                animation->m_QuadRenderer->m_EventCallback = nullptr;
                animation->m_ElapsedTime = 0;
            }
            if (auto tr = dynamic_pointer_cast<TextRendererComponent>(component)) {
                tr->RemoveRender();
                tr->m_EventCallback = nullptr;
            }
        }

        m_EventCallback = nullptr;
        End();
    }

    void Entity::OnUpdate(float deltaTime) {

        // Transform Updates
        if (m_Transform.get()->m_Modified) {
            for (auto component : m_Components) {
                if (auto qr = dynamic_pointer_cast<QuadRendererComponent>(component)) {
                    qr->UpdateModelMatrix(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
                }
                else if (auto camera = dynamic_pointer_cast<CameraComponent>(component)) {
                    camera->UpdateCameraTransform(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
                }
                else if (auto collider = dynamic_pointer_cast<ColliderComponent>(component)) {
                    collider->UpdateTransform();
                }
                else if (auto tileMap = dynamic_pointer_cast<TileMapComponent>(component)) {
                    tileMap->UpdateTransform(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
                }
                else if (auto animation = dynamic_pointer_cast<AnimationComponent>(component)) {
                    animation->m_QuadRenderer->UpdateModelMatrix(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
                }
                else if (auto tr = dynamic_pointer_cast<TextRendererComponent>(component)) {
                    tr->UpdateModelMatrix(m_Transform.get()->Position(), m_Transform.get()->Rotation(), m_Transform.get()->Scale());
                }
            }
            for (auto child : m_Children) {
                child->m_Transform.get()->m_Modified = true;
            }

            m_Transform.get()->m_Modified = false;
        }

        // Any per frame updates for components
        for (auto component : m_Components) {
            if (auto animation = dynamic_pointer_cast<AnimationComponent>(component)) {
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