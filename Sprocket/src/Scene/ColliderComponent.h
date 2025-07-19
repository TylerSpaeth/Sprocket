#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "Component.h"
#include "TransformComponent.h"

#include "Events/Event.h"
#include "Events/PhysicsEvent.h"

#include "Core/Macros.h"

#include "ThirdParty/glm/glm.hpp"

#include <functional>

namespace Sprocket {

  // TODO Provide implementations for the pure virtual functions that are the same in both
  // child classes
  class SPROCKET_API ColliderComponent : public Component {
    friend class Entity;
    protected:
      int m_PhysicsID = -1;
      std::function<void(Event&)> m_EventCallback;
      TransformComponent* m_TranformComponent = nullptr;
      ColliderComponent(TransformComponent& transformComponent) : m_TranformComponent(&transformComponent) {}

      
      virtual void Register() = 0;
      virtual void UpdateTransform() = 0;
      void Remove();
    public:

      int GetPhysicsID() const {return m_PhysicsID;}

      bool CollidesWith(ColliderComponent& collider);

  };

  class SPROCKET_API BoxColliderComponent : public ColliderComponent {
    friend class Entity;
    private:
      glm::vec2 m_Size = {1,1};
      BoxColliderComponent(TransformComponent& transformComponent) : ColliderComponent(transformComponent) {}

      
      void Register() override;
      void UpdateTransform() override;

    public:
      glm::vec2 GetSize() const;
      void SetSize(const glm::vec2 size);
  };

  class SPROCKET_API CircleColliderComponent : public ColliderComponent {
    friend class Entity;
    private:
      float m_Radius = 1;
      CircleColliderComponent(TransformComponent& transformComponent) : ColliderComponent(transformComponent) {}

      
      void Register() override;
      void UpdateTransform() override;

    public:
      float GetRadius() const;
      void SetRadius(float radius);
  };
}

#endif