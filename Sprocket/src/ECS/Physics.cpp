#include "ECS/Physics.h"
#include "ECS/Collision.h"

#include "Events/ApplicationEvent.h"

#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace Sprocket {

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////// EVENT HANDLING /////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  void Physics::OnUpdate(float deltaTime) {

    ClearPreviousCollisions();

    ProcessCollisions();

    // TODO
  }

  void Physics::OnEvent(Event& e) {
    switch(e.GetEventType()) {
      case EventType::APP_UPDATE:
        OnUpdate(((ApplicationUpdateEvent&)e).GetDeltaTime());
        break;
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////// REGISTRATION //////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  bool Physics::RegisterNewPhysicsObject(TransformComponent& localtcomp, TransformComponent& globaltcomp, PhysicsComponent& pcomp) {
    
    if(pcomp.phyiscsID != -1) {
      return false;
    }

    // Create a new physics object and assign its transform and physics components
    PhysicsObject object;
    object.m_LocalTransform = &localtcomp;
    object.m_GlobalTransform = &globaltcomp;
    object.m_Physics = &pcomp;

    // If there is a deleted object that has not been replaced
    if(m_DeletedPhysicsObjects.size() != 0) {
      // Find the deleted object closest to the start of the vector
      auto freeSlot = m_DeletedPhysicsObjects.top();
      m_DeletedPhysicsObjects.pop();
      // Assign that free slot index as the id of the PhysicsComponent
      pcomp.phyiscsID = freeSlot;
      // Replace the delted object with this new object
      m_Objects.at(freeSlot) = object;
    }

    // Otherwise if the vector is contiguous from 0 to size(), then put the new object at the back
    else {
      pcomp.phyiscsID = m_Objects.size();
      m_CollidesWith.push_back(std::vector<unsigned int>());
      m_Objects.push_back(object);
    }

    return true;

  }

  bool Physics::RegisterNewPhysicsObject(TransformComponent& localtcomp, TransformComponent& globaltcomp, PhysicsComponent& pcomp, ColliderComponent& ccomp) {

    // Register the physics object without the collider and then add the collider after based on 
    // the physicsID that gets assigned at registration.
    if(!RegisterNewPhysicsObject(localtcomp,globaltcomp,pcomp)) return false;

    // Should setting the collider fail, delete the object to prevent many partial objects being 
    // created without the user realizing.
    if(!SetCollider(pcomp.phyiscsID, ccomp)) {
      DeletePhysicsObject(pcomp.phyiscsID);
      return false;
    }

    return true;

  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////// DELETION ////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  bool Physics::DeletePhysicsObject(const int physicsID) {

    if(physicsID < 0 || physicsID >= m_Objects.size()) return false;

    auto object = &m_Objects.at(physicsID);

    if(object->m_Physics == nullptr) return false;

    // Reset the physicsID to unassigned
    object->m_Physics->phyiscsID = -1;

    // Having all four of these set to nullptr indicates deleted
    object->m_Collider = nullptr;
    object->m_Physics = nullptr;
    object->m_LocalTransform = nullptr;
    object->m_GlobalTransform = nullptr;

    return true;

  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////// COLLIDERS ///////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  bool Physics::SetCollider(const int physicsID, ColliderComponent& ccomp) {

    if(physicsID < 0 || physicsID >= m_Objects.size()) return false;

    auto object = &m_Objects.at(physicsID);

    if(object->m_Physics == nullptr) return false;

    m_Objects.at(physicsID).m_Collider = &ccomp;

    return true;

  }

  bool Physics::RemoveCollider(const int physicsID) {

    if(physicsID < 0 || physicsID >= m_Objects.size()) return false;

    auto object = &m_Objects.at(physicsID);

    if(object->m_Physics == nullptr) return false;

    m_Objects.at(physicsID).m_Collider = nullptr;

    return true;

  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  void Physics::ClearPreviousCollisions() {
    for(int i = 0; i < m_CollidesWith.size(); i++) {
      m_CollidesWith.at(i).clear();
    }
  }

  void Physics::ProcessCollisions() {
    // LOOKHERE this is currently implemented is a naive solution. This is quite a poor solution and
    // and should be changed.

    // Compare every dynamic object to every other object except itself
    for(int i = 0; i < m_Objects.size(); i++) {

      if(m_Objects.at(i).m_Physics == nullptr) continue;
      // If the object is not dynamic, no need to check further
      if(m_Objects.at(i).m_Physics->isDynamic == false) continue;

      auto obj1 = m_Objects.at(i);
      if(obj1.m_Collider == nullptr) break;
      // If true, obj1 has a box collider, if false then circle collider
      bool boxCollider1 = obj1.m_Collider->isBoxCollider;

      TransformComponent obj1ComputedTransform = *obj1.m_GlobalTransform;
      obj1ComputedTransform.position += obj1.m_LocalTransform->position;
      obj1ComputedTransform.rotation += obj1.m_LocalTransform->rotation;
      obj1ComputedTransform.scale *= obj1.m_LocalTransform->scale;

      for(int j = 0; j < m_Objects.size(); j++) {

        if(m_Objects.at(j).m_Physics == nullptr) break;
        if(j == i) break;

        auto obj2 = m_Objects.at(j);
        if(obj2.m_Collider == nullptr) break;
        // If true, obj2 has a box collider, if false then circle collider
        bool boxCollider2 = obj2.m_Collider->isBoxCollider;

        TransformComponent obj2ComputedTransform = *obj2.m_GlobalTransform;
        obj2ComputedTransform.position += obj2.m_LocalTransform->position;
        obj2ComputedTransform.rotation += obj2.m_LocalTransform->rotation;
        obj2ComputedTransform.scale *= obj2.m_LocalTransform->scale;

        // Check box-box collision
        if(boxCollider1 && boxCollider2) {

          // If there is a collision between the boxes
          if(Collision::Collides(*(BoxColliderComponent*)obj1.m_Collider, obj1ComputedTransform, *(BoxColliderComponent*)obj2.m_Collider, obj2ComputedTransform)) {
            
            // Update the m_CollidesWith vector so that the vectors corresponding to each object are
            // are updated to collide with each other.
            m_CollidesWith.at(obj1.m_Physics->phyiscsID).push_back(obj2.m_Physics->phyiscsID);
            m_CollidesWith.at(obj2.m_Physics->phyiscsID).push_back(obj1.m_Physics->phyiscsID);

            //TODO handle any transform updates that are needed as a result of a collision
          }

        }

        // Check box-circle collision
        else if(boxCollider1 && !boxCollider2) {
          
          // If there is a collision between the shapes
          if(Collision::Collides(*(BoxColliderComponent*)obj1.m_Collider, obj1ComputedTransform, *(CircleColliderComponent*)obj2.m_Collider, obj2ComputedTransform)) {

            // Update the m_CollidesWith vector so that the vectors corresponding to each object are
            // are updated to collide with each other.
            m_CollidesWith.at(obj1.m_Physics->phyiscsID).push_back(obj2.m_Physics->phyiscsID);
            m_CollidesWith.at(obj2.m_Physics->phyiscsID).push_back(obj1.m_Physics->phyiscsID);

            //TODO handle any transform updates that are needed as a result of a collision
          }

        }

        // Check circle-box collision
        else if(boxCollider2 && !boxCollider1) {

          // If there is a collision between the shapes
          if(Collision::Collides(*(CircleColliderComponent*)obj1.m_Collider, obj1ComputedTransform, *(BoxColliderComponent*)obj2.m_Collider, obj2ComputedTransform)) {

            // Update the m_CollidesWith vector so that the vectors corresponding to each object are
            // are updated to collide with each other.
            m_CollidesWith.at(obj1.m_Physics->phyiscsID).push_back(obj2.m_Physics->phyiscsID);
            m_CollidesWith.at(obj2.m_Physics->phyiscsID).push_back(obj1.m_Physics->phyiscsID);

            //TODO handle any transform updates that are needed as a result of a collision
          }

        }

        // Check circle-circle collision
        else {

          // If there is a collision between the circles
          if(Collision::Collides(*(CircleColliderComponent*)obj1.m_Collider, obj1ComputedTransform, *(CircleColliderComponent*)obj2.m_Collider, obj2ComputedTransform)) {

            // Update the m_CollidesWith vector so that the vectors corresponding to each object are
            // are updated to collide with each other.
            m_CollidesWith.at(obj1.m_Physics->phyiscsID).push_back(obj2.m_Physics->phyiscsID);
            m_CollidesWith.at(obj2.m_Physics->phyiscsID).push_back(obj1.m_Physics->phyiscsID);

            //TODO handle any transform updates that are needed as a result of a collision
          }

        }
        
      }

    }
  }

  // TODO error checking
  // Note that this function only returns a valid value when used on a dynamic physics object
  int Physics::CountCollisions(const int physiscsID) { 
    try {
      return m_CollidesWith.at(physiscsID).size();
    }
    catch(const std::exception& e) {
      return 0;
    }

  }

  // TODO error checking
  // Note that this function only returns a valid value when used on a dynamic physics object
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