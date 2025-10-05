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

    bool Entity::InitSelf(std::shared_ptr<Entity> self) {
        if (self.get() != this) {
            Global::fileLogger.Warning("InitSelf Failed. Provided pointer does not point to self.");
            return false;
        }
        m_Self = self; // call this right after construction
        return true;
    }

    Entity::Entity() {
        auto parentGlobalTransform = GetParentGlobalTransform();
        m_Transform = std::make_shared<TransformComponent>([this]() {return GetParentGlobalTransform(); });
        InitializeAllowedComponents();
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
        if (!m_Parent.lock()) return TransformComponent(nullptr);

        TransformComponent ret(nullptr);
        auto lockedParent = m_Parent.lock();
        ret.m_LocalPosition = lockedParent->m_Transform->Position();
        ret.m_LocalRotation = lockedParent->m_Transform->Rotation();
        ret.m_LocalScale = lockedParent->m_Transform->Scale();
        return ret;
    }

    void Entity::OnEvent(Event& event) {}

    void Entity::OnActivate() {

        OnActivateParams params;
        params.eventCallback = m_EventCallback;
        params.position = m_Transform->Position();
        params.rotation = m_Transform->Rotation();
        params.scale = m_Transform->Scale();

        for (auto component : m_Components) {
            component->OnActivate(params);
        }
        Start();
    }

    void Entity::OnDeactivate() {

        OnDeactivateParams params;

        for (auto component : m_Components) {
            component->OnDeactivate(params);
        }

        m_EventCallback = nullptr;
        End();
    }

    void Entity::OnUpdate(float deltaTime) {

        OnUpdateParams params;
        params.deltaTime = deltaTime;
        if (m_Transform->m_Modified) {
            params.updatedTransform = true;
            params.position = m_Transform->Position();
            params.rotation = m_Transform->Rotation();
            params.scale = m_Transform->Scale();
        }

        for (auto component : m_Components) {

            component->OnUpdate(params);

            if (m_Transform->m_Modified) {
                for (auto child : m_Children) {
                    if (auto lockedChild = child.lock()) {
                        lockedChild->m_Transform->m_Modified = true;
                    }
                }
            }
        }

        m_Transform->m_Modified = false;

        Update(deltaTime);

    }

    void Entity::InitializeAllowedComponents() {
        // Transform is set to 0 since that can never be added to an entity
        m_AllowedComponents.insert({typeid(TransformComponent), std::make_shared<unsigned int>(0)});
        m_AllowedComponents.insert({typeid(CameraComponent), std::make_shared<unsigned int>(1) });
        auto maximumColliders = std::make_shared<unsigned int>(1);
        m_AllowedComponents.insert({typeid(ColliderComponent), maximumColliders});
        m_AllowedComponents.insert({typeid(BoxColliderComponent), maximumColliders});
        m_AllowedComponents.insert({typeid(CircleColliderComponent), maximumColliders});
        auto maximumRenderers = std::make_shared<unsigned int>(1);
        m_AllowedComponents.insert({typeid(QuadRendererComponent), maximumRenderers});
        m_AllowedComponents.insert({typeid(AnimationComponent), maximumRenderers});
        m_AllowedComponents.insert({typeid(TextRendererComponent), maximumRenderers});
        m_AllowedComponents.insert({typeid(SoundComponent), std::make_shared<unsigned int>(1)});
        m_AllowedComponents.insert({typeid(TileMapComponent), std::make_shared<unsigned int>(1)});
    }

}