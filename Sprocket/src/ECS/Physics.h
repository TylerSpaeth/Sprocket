#ifndef PHYSICS_H
#define PHYSICS_H

#include "ECS/Component.h"

#include "Events/Event.h"

#include <queue>
#include <vector>
#include <optional>

namespace Sprocket {

  class PhysicsObject {
    friend class Physics;
    private:
      TransformComponent m_Transform;
      PhysicsComponent m_Physics;

      // The appropriate variable is used based on the type of collider that is assigned
      std::optional<BoxColliderComponent> m_BCollider;
      std::optional<CircleColliderComponent> m_CCollider;

      PhysicsObject(TransformComponent& tcomp, PhysicsComponent& pcomp) : m_Transform(tcomp), m_Physics(pcomp){}
      PhysicsObject(TransformComponent& tcomp, PhysicsComponent& pcomp, ColliderComponent& ccomp) : m_Transform(tcomp), m_Physics(pcomp) {
        if(ccomp.isBoxCollider) {
          m_BCollider = BoxColliderComponent((BoxColliderComponent&)ccomp);
        }
        else {
          m_CCollider = CircleColliderComponent((CircleColliderComponent&)ccomp);
        }
      }
  };

  class Physics {
    friend class Scene;
    private:
      
      Physics();

      std::vector<PhysicsObject> m_Objects;
      std::vector<std::vector<unsigned int>> m_CollidesWith;

      std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_FreeSlots;

      void OnUpdate(float deltaTime);
      void ClearPreviousCollisions();
      void ProcessCollisions();

    public:

      /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
      /// instead be called directly be the SceneManager when it recieves an event.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);

      bool RegisterNewPhysicsObject(TransformComponent& transform, PhysicsComponent& pcomp);
      bool RegisterNewPhysicsObject(TransformComponent& transform, PhysicsComponent& pcomp, ColliderComponent& ccomp);

      bool DeletePhysicsObject(const int physicsID);

      bool UpdateTransform(const int physicsID, TransformComponent& transform);

      bool SetCollider(const int physicsID, ColliderComponent& ccomp);
      bool RemoveCollider(const int physicsID);

      int CountCollisions(const int physiscsID);
      bool CollidesWith(const int physicsID, const int otherPhysicsID);

  };

}

#endif