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
      
      // BROAD PHASE DETECTION

      /// @brief A helper function that adds the object to the region sets adds the region for 
      /// reverse lookup.
      /// @param coordinates The grid coordinate that should be associated with the object
      /// @param physicsID The id of the object to use.
      void SetRegion(std::pair<int,int> coordinates, const int physicsID);
      /// @brief Calculates what regions this object belongs in and places it in all of them.
      /// @param physicsID The id of the object to place.
      void PlaceInRegions(const int physicsID);
      /// @brief Removes the object from all regions.
      /// @param physicsID The id of the object to be removed.
      void RemoveFromRegions(const int physicsID);

      // NARROW PHASE DETECTION
      
      /// @brief Clears out the m_CollidesWith vector. This needs to be called before 
      /// ProcessCollisions(), otherwise residual collision data from the previous frame will
      /// corrupt the data of the current frame.
      void ClearPreviousCollisions();
      /// @brief Iterates over all of the regions in m_Regions and performs narrow phase collision 
      /// detection on all of the object contained in each region, updating m_CollidesWith.
      void ProcessCollisions();
      /// @brief A helper function to update the m_CollidesWith vector to reflect a collision 
      /// between the two given physics objects. This function provides no input validation,
      /// so it is up to the caller to ensure that if this function is called, the two objects
      /// have actually collided.
      /// @param physicsID1 The id of the first object.
      /// @param physicsID2 The id of the second object.
      void UpdateCollidesWith(const int physicsID1, const int physicsID2);

    public:

      /// @brief Handles incoming events.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);

      /// @brief Registers the given physics and transform components into the physics system as an 
      /// object. The physicsID variable of the pcomp reference will be set to the proper ID to 
      /// access this object in the future.
      /// @param transform The transform component for this new physics object.
      /// @param pcomp The physics component for this new physics object.
      /// @return true if registering is successful, false otherwise.
      bool RegisterNewPhysicsObject(TransformComponent& transform, PhysicsComponent& pcomp);
      /// @brief Registers the given physics, collider and transform components into the physics 
      /// system as an object. The physicsID variable of the pcomp reference will be set to the 
      /// proper ID to access this object in the future.
      /// @param transform The transform component for this new physics object.
      /// @param pcomp The physics component for this new physics object.
      /// @param ccomp The collider component for this new physics object.
      /// @return true if registering is successful, false otherwise.
      bool RegisterNewPhysicsObject(TransformComponent& transform, PhysicsComponent& pcomp, ColliderComponent& ccomp);

      /// @brief Deletes the object registered to the given physicsID.
      /// @param physicsID The id of the object to be deleted.
      /// @return true if the deletion is successful, false otherwise.
      bool DeletePhysicsObject(const int physicsID);

      /// @brief Updates the transform component of the object with the given physicsID.
      /// @param physicsID The id of the appropriate object.
      /// @param transform The transform component that should be set for the object.
      /// @return true if the update is successful, false otherwise.
      bool UpdateTransform(const int physicsID, TransformComponent& transform);

      /// @brief Sets a new collider for the object with the given physicsID
      /// @param physicsID The id of the object that the collider should be attached to.
      /// @param ccomp The collider component to attach to the object.
      /// @return true if setting the collider is successful, false if not.
      bool SetCollider(const int physicsID, ColliderComponent& ccomp);
      /// @brief Removes the collider object from the object corresponding to the given ID.
      /// @param physicsID The id corresponding to the object that should have its collider removed
      /// @return true if the removal is successful, false otherwise
      bool RemoveCollider(const int physicsID);

      /// @brief Counts the number of other objects that this collider is object is colliding with
      /// @param physiscsID The if of the object
      /// @return the number of other objects that this one collides with
      int CountCollisions(const int physiscsID);
      /// @brief Checks if there is a collision between the two given objects.
      /// @param physicsID The id of the first object
      /// @param otherPhysicsID The id of the second object
      /// @return true if the two objects are colliding, and false if they are not
      bool CollidesWith(const int physicsID, const int otherPhysicsID);

  };

}

#endif