#include "ECS/Physics.h"

#include "ECS/Collision.h"

#include "Events/ApplicationEvent.h"

#include <algorithm>
#include <iostream>

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

  void Physics::ProcessCollisions() {

    // TODO clean this up. It is terrible to read.

    // LOOKHERE this is currently implemented is a naive solution. This is quite a poor solution and
    // and should be changed.

    // Compare every dynamic object to every other object except itself
    for(int i = 0; i < m_Objects.size(); i++) {

      if(m_Objects.at(i).m_Physics.physicsID == -1) continue;
      // If the object is not dynamic, no need to check further
      if(m_Objects.at(i).m_Physics.isDynamic == false) continue;

      auto obj1 = m_Objects.at(i);

      // If the object has not collider
      if(!obj1.m_BCollider.has_value() && !obj1.m_CCollider.has_value()) break;


      // If true, obj1 has a box collider, if false then circle collider
      bool boxCollider1 = obj1.m_BCollider.has_value();

      TransformComponent obj1Transform = obj1.m_Transform;

      for(int j = 0; j < m_Objects.size(); j++) {

        if(m_Objects.at(j).m_Physics.physicsID == -1) break;
        if(j == i) break;

        auto obj2 = m_Objects.at(j);

        // If the object has not collider
        if(!obj2.m_BCollider.has_value() && !obj2.m_CCollider.has_value()) break;

        // If true, obj2 has a box collider, if false then circle collider
        bool boxCollider2 = obj2.m_BCollider.has_value();

        TransformComponent obj2Transform = obj2.m_Transform;

        // Check box-box collision
        if(boxCollider1 && boxCollider2) {

          // If there is a collision between the boxes
          if(Collision::Collides(obj1.m_BCollider.value(), obj1Transform, obj2.m_BCollider.value(), obj2Transform)) {
            
            // Update the m_CollidesWith vector so that the vectors corresponding to each object are
            // are updated to collide with each other.
            m_CollidesWith.at(obj1.m_Physics.physicsID).push_back(obj2.m_Physics.physicsID);
            m_CollidesWith.at(obj2.m_Physics.physicsID).push_back(obj1.m_Physics.physicsID);

            //TODO handle any transform updates that are needed as a result of a collision
          }

        }

        // Check box-circle collision
        else if(boxCollider1 && !boxCollider2) {
          
          // If there is a collision between the shapes
          if(Collision::Collides(obj1.m_BCollider.value(), obj1Transform, obj2.m_CCollider.value(), obj2Transform)) {

            // Update the m_CollidesWith vector so that the vectors corresponding to each object are
            // are updated to collide with each other.
            m_CollidesWith.at(obj1.m_Physics.physicsID).push_back(obj2.m_Physics.physicsID);
            m_CollidesWith.at(obj2.m_Physics.physicsID).push_back(obj1.m_Physics.physicsID);

            //TODO handle any transform updates that are needed as a result of a collision
          }

        }

        // Check circle-box collision
        else if(boxCollider2 && !boxCollider1) {

          // If there is a collision between the shapes
          if(Collision::Collides(obj1.m_CCollider.value(), obj1Transform, obj2.m_BCollider.value(), obj2Transform)) {

            // Update the m_CollidesWith vector so that the vectors corresponding to each object are
            // are updated to collide with each other.
            m_CollidesWith.at(obj1.m_Physics.physicsID).push_back(obj2.m_Physics.physicsID);
            m_CollidesWith.at(obj2.m_Physics.physicsID).push_back(obj1.m_Physics.physicsID);

            //TODO handle any transform updates that are needed as a result of a collision
          }

        }

        // Check circle-circle collision
        else {

          // If there is a collision between the circles
          if(Collision::Collides(obj1.m_CCollider.value(), obj1Transform, obj2.m_CCollider.value(), obj2Transform)) {

            // Update the m_CollidesWith vector so that the vectors corresponding to each object are
            // are updated to collide with each other.
            m_CollidesWith.at(obj1.m_Physics.physicsID).push_back(obj2.m_Physics.physicsID);
            m_CollidesWith.at(obj2.m_Physics.physicsID).push_back(obj1.m_Physics.physicsID);

            //TODO handle any transform updates that are needed as a result of a collision
          }

        }
        
      }

    }

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
    
    return true;
  }

  bool Physics::DeletePhysicsObject(const int physicsID) {

    if(physicsID < 0 || physicsID >= m_Objects.size()) return false;

    // Invalidate the ID of the object at the given slot
    m_Objects.at(physicsID).m_Physics.physicsID = -1;

    m_CollidesWith.at(physicsID).clear();

    m_FreeSlots.push(physicsID);

    return true;
  }

  bool Physics::UpdateTransform(const int physicsID, TransformComponent& transform) {

    if(physicsID < 0 || physicsID >= m_Objects.size()) return false;

    m_Objects.at(physicsID).m_Transform = transform;
    
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