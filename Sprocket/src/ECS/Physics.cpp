#include "ECS/Physics.h"

#include "ECS/Collision.h"

#include "Events/ApplicationEvent.h"

#include <algorithm>
#include <iostream>
#include <limits>

namespace Sprocket {

  Physics::Physics() {

    // The first slot will always be free in a new vector
    m_FreeSlots.push(0);

  }

  void Physics::OnUpdate(float deltaTime) {
    
    ClearPreviousCollisions();

    ProcessCollisions();

  }

  void Physics::OnEvent(Event& event) {
    switch(event.GetEventType()) {
      case EventType::APP_UPDATE:
        OnUpdate(((ApplicationUpdateEvent&)event).GetDeltaTime());
        break;
    }
  }

  void Physics::ClearPreviousCollisions() {

    for(int i = 0; i < m_CollidesWith.size(); i++) {
      m_CollidesWith.at(i).clear();
    }

  }

  void Physics::SetRegion(std::pair<int,int> coordinates, const int physicsID) {
    if(!m_Regions.count(coordinates)) {
      m_Regions.insert({coordinates, std::vector<unsigned int>()});
    }
    m_Regions.at(coordinates).push_back(physicsID);
    m_ReverseRegions.at(physicsID).push_back(coordinates);
  }

  void Physics::PlaceInRegions(const int physicsID) {

    auto obj = m_Objects.at(physicsID);
    bool boxCollider = obj.m_BCollider.has_value();

    int startXRegion = obj.m_Transform.position.x / m_BoxXSize;
    int startYRegion = obj.m_Transform.position.y / m_BoxYSize;

    // The object must collider with the region where its center is so there is no need to check
    // for the collision.
    SetRegion({startXRegion,startYRegion}, physicsID);

    // The number of grid regions in width and height to be checked on the current pass
    int searchGridSize = 3;

    bool collisionDetected = true;
    BoxColliderComponent bcomp;
    bcomp.width = m_BoxXSize;
    bcomp.height = m_BoxYSize;

    // Continue looping so long as a there was a collision found in the previous iteration
    while(collisionDetected) {
      
      collisionDetected = false;
      int tmp;

      // Run along the right edge
      int xRegion = startXRegion + (searchGridSize-1)/2;
      int yRegion = startYRegion + (searchGridSize-1)/2;

      tmp = yRegion-searchGridSize;
      while(yRegion > tmp) {
        
        TransformComponent tcomp;
        tcomp.position.x = xRegion * m_BoxXSize;
        tcomp.position.y = yRegion * m_BoxYSize;

        if(boxCollider) {
          if(Collision::Collides(obj.m_BCollider.value(), obj.m_Transform, bcomp, tcomp)) {
            SetRegion({xRegion,yRegion}, physicsID);

            collisionDetected = true;
          }
        }
        else {
          if(Collision::Collides(obj.m_CCollider.value(), obj.m_Transform, bcomp, tcomp)) {
            SetRegion({xRegion,yRegion}, physicsID);

            collisionDetected = true;
          }
        }

        yRegion--;
      }

      // Run along the bottom edge
      tmp = xRegion-searchGridSize;
      while(xRegion > tmp) {

        TransformComponent tcomp;
        tcomp.position.x = xRegion * m_BoxXSize;
        tcomp.position.y = yRegion * m_BoxYSize;

        if(boxCollider) {
          if(Collision::Collides(obj.m_BCollider.value(), obj.m_Transform, bcomp, tcomp)) {
            SetRegion({xRegion,yRegion}, physicsID);

            collisionDetected = true;
          }
        }
        else {
          if(Collision::Collides(obj.m_CCollider.value(), obj.m_Transform, bcomp, tcomp)) {
            SetRegion({xRegion,yRegion}, physicsID);

            collisionDetected = true;
          }
        }

        xRegion--;
      }

      // Run along left edge
      tmp = yRegion+searchGridSize;
      while(yRegion < tmp) {
        
        TransformComponent tcomp;
        tcomp.position.x = xRegion * m_BoxXSize;
        tcomp.position.y = yRegion * m_BoxYSize;

        if(boxCollider) {
          if(Collision::Collides(obj.m_BCollider.value(), obj.m_Transform, bcomp, tcomp)) {
            SetRegion({xRegion,yRegion}, physicsID);

            collisionDetected = true;
          }
        }
        else {
          if(Collision::Collides(obj.m_CCollider.value(), obj.m_Transform, bcomp, tcomp)) {
            SetRegion({xRegion,yRegion}, physicsID);

            collisionDetected = true;
          }
        }

        yRegion++;
      }

      // Run along the top edge
      tmp = xRegion+searchGridSize;
      while(xRegion < tmp) {

        TransformComponent tcomp;
        tcomp.position.x = xRegion * m_BoxXSize;
        tcomp.position.y = yRegion * m_BoxYSize;

        if(boxCollider) {
          if(Collision::Collides(obj.m_BCollider.value(), obj.m_Transform, bcomp, tcomp)) {
            SetRegion({xRegion,yRegion}, physicsID);;

            collisionDetected = true;
          }
        }
        else {
          if(Collision::Collides(obj.m_CCollider.value(), obj.m_Transform, bcomp, tcomp)) {
            SetRegion({xRegion,yRegion}, physicsID);

            collisionDetected = true;
          }
        }

        xRegion++;
      }

      // Expand the grid to check 1 more grid cell on each size
      searchGridSize += 2;

    }

  }
  
  // TODO initialize a reverse region vector for all new physicsIDs
  void Physics::RemoveFromRegions(const int physicsID) {
    
    auto regions = m_ReverseRegions.at(physicsID);

    for(auto region : regions) {
      auto it = std::find(m_Regions.at(region).begin(), m_Regions.at(region).end(), physicsID);
      m_Regions.at(region).erase(it);
      
      auto it2 = std::find(m_ReverseRegions.at(physicsID).begin(), m_ReverseRegions.at(physicsID).end(), region);
      m_ReverseRegions.at(physicsID).erase(it2);

    }

  }

  void Physics::ProcessCollisions() {

    for(auto cell : m_Regions) {

      auto objects = cell.second;

      for(auto objID1 : objects) {
        for(auto objID2 = std::find(objects.begin(), objects.end(), objID1); objID2 != objects.end(); objID2++) {
          if(objID1 == *objID2) continue;

          auto obj1 = m_Objects.at(objID1);
          auto obj2 = m_Objects.at(*objID2);

          // If true, obj1 has a box collider, if false then circle collider
          bool boxCollider1 = obj1.m_BCollider.has_value();
          // If true, obj2 has a box collider, if false then circle collider
          bool boxCollider2 = obj2.m_BCollider.has_value();

          TransformComponent obj1Transform = obj1.m_Transform;
          TransformComponent obj2Transform = obj2.m_Transform;

          // Check box-box collision
          if(boxCollider1 && boxCollider2) {

            // If there is a collision between the boxes
            if(Collision::Collides(obj1.m_BCollider.value(), obj1Transform, obj2.m_BCollider.value(), obj2Transform)) {
                
              UpdateCollisions(obj1.m_Physics.physicsID, obj2.m_Physics.physicsID);

              //TODO handle any transform updates that are needed as a result of a collision
            }

          }

          // Check box-circle collision
          else if(boxCollider1 && !boxCollider2) {
              
            // If there is a collision between the shapes
            if(Collision::Collides(obj1.m_BCollider.value(), obj1Transform, obj2.m_CCollider.value(), obj2Transform)) {

              UpdateCollisions(obj1.m_Physics.physicsID, obj2.m_Physics.physicsID); 

              //TODO handle any transform updates that are needed as a result of a collision
            }

          }

          // Check circle-box collision
          else if(boxCollider2 && !boxCollider1) {

            // If there is a collision between the shapes
            if(Collision::Collides(obj1.m_CCollider.value(), obj1Transform, obj2.m_BCollider.value(), obj2Transform)) {

              UpdateCollisions(obj1.m_Physics.physicsID, obj2.m_Physics.physicsID);

              //TODO handle any transform updates that are needed as a result of a collision
            }

          }

          // Check circle-circle collision
          else {

            // If there is a collision between the circles
            if(Collision::Collides(obj1.m_CCollider.value(), obj1Transform, obj2.m_CCollider.value(), obj2Transform)) {

              UpdateCollisions(obj1.m_Physics.physicsID, obj2.m_Physics.physicsID);

              //TODO handle any transform updates that are needed as a result of a collision
            }

          }

        }
      }

    }

  }

  // Update the m_CollidesWith vector so that the vectors corresponding to each object are
  // are updated to collide with each other.
  void Physics::UpdateCollisions(const int physicsID1, const int physicsID2) {
    m_CollidesWith.at(physicsID1).push_back(physicsID2);
    m_CollidesWith.at(physicsID2).push_back(physicsID1);
  }

  bool Physics::RegisterNewPhysicsObject(TransformComponent& transform, PhysicsComponent& pcomp) {

    // Ensure this has an undefined ID
    if(pcomp.physicsID != -1) {
      return false;
    }

    PhysicsObject object(transform, pcomp);

    auto freeslot = m_FreeSlots.top();
    
    // If there is only 1 entry in the queue then it must be at the end of the vector, so the next 
    // free slot needs to be added
    if(m_FreeSlots.size() == 1) {
      m_FreeSlots.push(freeslot+1);
    }

    m_FreeSlots.pop();

    // Assign the ID to the physics objects
    pcomp.physicsID = freeslot;
    object.m_Physics.physicsID = freeslot;

    // Put the object in the correct slot and setup the collides with vector
    if(freeslot == m_Objects.size()) {
      m_Objects.push_back(object);
      m_CollidesWith.push_back(std::vector<unsigned int>());
      m_ReverseRegions.push_back(std::vector<std::pair<int,int>>());
    }
    else {
      m_Objects.at(freeslot) = object;
      m_CollidesWith.at(freeslot).clear();
    }

    return true;
  }

  bool Physics::RegisterNewPhysicsObject(TransformComponent& transform, PhysicsComponent& pcomp, ColliderComponent& ccomp) {
    
    if(!RegisterNewPhysicsObject(transform,pcomp)) {
      return false;
    }

    // Assign the collider appropriately
    if(ccomp.isBoxCollider) {
      m_Objects.at(pcomp.physicsID).m_BCollider = BoxColliderComponent((BoxColliderComponent&)ccomp);
    }
    else {
      m_Objects.at(pcomp.physicsID).m_CCollider = CircleColliderComponent((CircleColliderComponent&)ccomp);
    }

    PlaceInRegions(pcomp.physicsID);

    return true;
  }

  bool Physics::DeletePhysicsObject(const int physicsID) {

    if(physicsID < 0 || physicsID >= m_Objects.size()) return false;

    // Invalidate the ID of the object at the given slot
    m_Objects.at(physicsID).m_Physics.physicsID = -1;

    m_CollidesWith.at(physicsID).clear();

    m_FreeSlots.push(physicsID);

    RemoveFromRegions(physicsID);

    return true;
  }

  bool Physics::UpdateTransform(const int physicsID, TransformComponent& transform) {

    if(physicsID < 0 || physicsID >= m_Objects.size()) return false;

    m_Objects.at(physicsID).m_Transform = transform;

    RemoveFromRegions(physicsID);
    PlaceInRegions(physicsID);

    return true;
  }

  bool Physics::SetCollider(const int physicsID, ColliderComponent& ccomp) {

    if(physicsID < 0 || physicsID >= m_Objects.size()) return false;

    // Assign the collider appropriately
    if(ccomp.isBoxCollider) {
      m_Objects.at(physicsID).m_BCollider = BoxColliderComponent((BoxColliderComponent&)ccomp);
      // Clear out the other collider in case it was previously set
      m_Objects.at(physicsID).m_CCollider.reset();
    }
    else {
      m_Objects.at(physicsID).m_CCollider = CircleColliderComponent((CircleColliderComponent&)ccomp);
      // Clear out the other collider in case it was previously set
      m_Objects.at(physicsID).m_BCollider.reset();
    }

    RemoveFromRegions(physicsID);
    PlaceInRegions(physicsID);

    return true;
  }

  bool Physics::RemoveCollider(const int physicsID) {

    if(physicsID < 0 || physicsID >= m_Objects.size()) return false;

    // If neither type of collider is assigned
    if(!m_Objects.at(physicsID).m_BCollider.has_value() && !m_Objects.at(physicsID).m_CCollider.has_value()) {
      return false;
    }
    
    // Reset both colliders
    m_Objects.at(physicsID).m_BCollider.reset();
    m_Objects.at(physicsID).m_CCollider.reset();

    RemoveFromRegions(physicsID);

    return true;
  }

  int Physics::CountCollisions(const int physicsID) {
    try {
      return m_CollidesWith.at(physicsID).size();
    }
    catch(const std::exception& e) {
      return 0;
    }
  }

  bool Physics::CollidesWith(const int physicsID, const int otherPhysicsID) {
    try {
      auto collisions = m_CollidesWith.at(physicsID);
      return std::find(collisions.begin(), collisions.end(), otherPhysicsID) != collisions.end();
    }
    catch(const std::exception& e) {
      return 0;
    }
  }

}