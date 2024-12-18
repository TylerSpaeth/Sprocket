#ifndef PHYSICS_H
#define PHYSICS_H

#include "ECS/Component.h"

#include "Events/Event.h"

#include <queue>
#include <vector>
#include <optional>
#include <map>

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

      // TODO these need to not be hard coded
      float m_BoxXSize = 200;
      float m_BoxYSize = 200;
      // Let the map be column number, row number, mapping to a vector of objectIDs that are in 
      // that grid
      std::map<std::pair<int,int>, std::vector<unsigned int>> m_Regions;
      // Mapping of objectIDs back to the regions that they are stored in
      std::vector<std::vector<std::pair<int,int>>> m_ReverseRegions;

      std::vector<PhysicsObject> m_Objects;
      std::vector<std::vector<unsigned int>> m_CollidesWith;

      std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_FreeSlots;

      void OnUpdate(float deltaTime);
      void ClearPreviousCollisions();

      // BROAD PHASE DETECTION
      void SetRegion(std::pair<int,int> coordinates, const int physicsID);
      void PlaceInRegions(const int physicsID);
      // Remove the object at this ID from all regions
      void RemoveFromRegions(const int physicsID);

      // NARROW PHASE DETECTION
      void ProcessCollisions();
      void UpdateCollisions(const int physicsID1, const int physicsID2);

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