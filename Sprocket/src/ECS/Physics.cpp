#include "ECS/Physics.h"

#include "Events/ApplicationEvent.h"

#include <stdexcept>

namespace Sprocket {

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////// EVENT HANDLING /////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  void Physics::OnUpdate(float deltaTime) {
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

}