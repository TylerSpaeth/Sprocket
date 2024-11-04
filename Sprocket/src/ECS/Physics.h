#ifndef PHYSICS_H
#define PHYSICS_H

#include "ECS/Component.h"
#include "Events/Event.h"

#include <vector>
#include <queue>

namespace Sprocket {

  class PhysicsObject {
    friend class Physics;
    private:
      TransformComponent* m_Transform;
      ColliderComponent* m_Collider;
      PhysicsComponent* m_Physics;
  };

  // There should be a seperate physics object for each scene. Unlike renderering, physics is 
  // tightly coupled to the ECS. This prevents the need for physics events from wastefully clogging
  // up the event system. It the future this could be changed by making a physics singleton owned 
  // by the SceneManager and then utilize physics events that the SceneManager does not allow to 
  // escape into the larger application.

  class Physics {
    friend class Scene;
    private:

      Physics(){}

      // TODO use some sort of spacial data structure to improve runtime
      std::vector<PhysicsObject> m_Objects;

      std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_DeletedPhysicsObjects;

      void OnUpdate(float deltaTime);

    public:

      /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
      /// instead be called directly be the Scene Manager when it recieves an event.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);
      
      bool RegisterNewPhysicsObject(TransformComponent& tcomp, PhysicsComponent& pcomp);
      bool RegisterNewPhysicsObject(TransformComponent& tcomp, PhysicsComponent& qcomp, ColliderComponent& ccomp);

      bool DeletePhysicsObject(const int physicsID);

      bool SetCollider(const int physicsID, ColliderComponent& ccomp);
      bool RemoveCollider(const int physicsID);

  };

}

#endif