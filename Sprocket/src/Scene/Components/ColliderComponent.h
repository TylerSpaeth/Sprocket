#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "Component.h"
#include "TransformComponent.h"

#include "Core/Macros.h"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {
    
    /// @brief The base class for all collider components. Not an actual collider itself.
    class SPROCKET_API ColliderComponent : public EventDrivenComponent {
        friend class Entity;
    protected:

        int m_PhysicsID = -1;
        TransformComponent* m_TranformComponent = nullptr;

    public:

        int GetPhysicsID() const;

        /// @brief Checks if there is a collision between this and the given collider.
        /// @param collider - The other collider to check for collisions against.
        /// @returns True if they collide, false otherwise
        bool CollidesWith(ColliderComponent& collider);

        /// @brief Checks if this collider collides with any other colliders.
        /// @returns True if there are any collisions, false otherwise
        bool CollidesWithAnything();

    protected:
        ColliderComponent(TransformComponent& transformComponent);
        virtual ~ColliderComponent();

        /// @brief Remove the collider from the physics system        
        void Remove();

        /// @brief Registers the collider with the physics system
        virtual void Register() = 0;

        /// @brief Updates the transform of the collider in the physics system
        virtual void UpdateTransform() = 0;

    };

    /// @brief A 2D box collider component.
    class SPROCKET_API BoxColliderComponent : public ColliderComponent {
        friend class Entity;
    private:
        glm::vec2 m_Size = { 1,1 };

    public:
        glm::vec2 GetSize() const;
        bool SetSize(const glm::vec2 size);

    private:
        BoxColliderComponent(TransformComponent& transformComponent);
        void Register() override;
        void UpdateTransform() override;
    };

    /// @brief A 2d circle collider component.
    class SPROCKET_API CircleColliderComponent : public ColliderComponent {
        friend class Entity;
    private:
        float m_Radius = 1;

    public:
        float GetRadius() const;
        bool SetRadius(float radius);

    private:
        CircleColliderComponent(TransformComponent& transformComponent);
        void Register() override;
        void UpdateTransform() override;

    };
}

#endif