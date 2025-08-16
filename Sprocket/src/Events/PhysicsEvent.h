#ifndef PHYSICSEVENT_H
#define PHYSICSEVENT_H

#include "Event.h"

#include "ThirdParty/glm/glm.hpp"

#include <vector>

namespace Sprocket {

  enum class ColliderType {
    BOX,
    CIRCLE
  };

  class PhysicsNewEvent : public Event {
    friend class Physics;
    friend class EventValidation;
    private:
      int m_PhysicsID = -1;
      glm::vec2 m_ColliderCenter;
      float m_CircleRadius = 0;
      float m_BoxColliderRotation = 0;
      glm::vec2 m_BoxColliderSize;
      ColliderType m_ColliderType;
    public:
      PhysicsNewEvent(glm::vec2 colliderCenter, glm::vec2 boxColliderSize, float boxColliderRotation) : m_ColliderCenter(colliderCenter), m_BoxColliderRotation(boxColliderRotation), m_BoxColliderSize(boxColliderSize) {
        this->SetType(EventType::PHYSICS_NEW);
        m_ColliderType = ColliderType::BOX;
      }
      PhysicsNewEvent(glm::vec2 colliderCenter, float circleRadius) : m_ColliderCenter(colliderCenter), m_CircleRadius(circleRadius) {
        this->SetType(EventType::PHYSICS_NEW);
        m_ColliderType = ColliderType::CIRCLE;
      }
      int GetPhysicsID() const {return m_PhysicsID;}
      ColliderType GetColliderType() const {return m_ColliderType;}
  };

  class PhysicsUpdateEvent : public Event {
    friend class Physics;
    friend class EventValidation;
    private:
      int m_PhysicsID = -1;
      glm::vec2 m_ColliderCenter;
      float m_CircleRadius = 0;
      float m_BoxColliderRotation = 0;
      glm::vec2 m_BoxColliderSize;
      ColliderType m_ColliderType;
    public:
      PhysicsUpdateEvent(const unsigned int physicsID, glm::vec2 colliderCenter, glm::vec2 boxColliderSize, float boxColliderRotation) : m_PhysicsID(physicsID), m_ColliderCenter(colliderCenter), m_BoxColliderRotation(boxColliderRotation), m_BoxColliderSize(boxColliderSize) {
        this->SetType(EventType::PHYSICS_UPDATE);
        m_ColliderType = ColliderType::BOX;
      }
      PhysicsUpdateEvent(const unsigned int physicsID, glm::vec2 colliderCenter, float circleRadius) : m_PhysicsID(physicsID), m_ColliderCenter(colliderCenter), m_CircleRadius(circleRadius) {
        this->SetType(EventType::PHYSICS_UPDATE);
        m_ColliderType = ColliderType::CIRCLE;
      }
      int GetPhysicsID() const {return m_PhysicsID;}
      ColliderType GetColliderType() const {return m_ColliderType;}
  };

  class PhysicsDeleteEvent : public Event {
    friend class Physics;
    friend class EventValidation;
    private:
      int m_PhysicsID = -1;
    public:
      PhysicsDeleteEvent(const unsigned int physicsID) : m_PhysicsID(physicsID) {
        this->SetType(EventType::PHYSICS_DELETE);
      }
      int GetPhysicsID() const {return m_PhysicsID;}
  };

  class PhysicsCollisionCheckEvent : public Event {
    friend class Physics;
    friend class EventValidation;
    private:
      int m_PhysicsID1 = -1;
      int m_PhysicsID2 = -1;
      bool m_Collides = false;
    public:
      PhysicsCollisionCheckEvent(const unsigned int physicsID1, const unsigned int physicsID2) : m_PhysicsID1(physicsID1), m_PhysicsID2(physicsID2) {
        this->SetType(EventType::PHYSICS_COLLISION_CHECK);
      }
      bool Collides() const {return m_Collides;}
  };

  class PhysicsCollisionCheckGenericEvent : public Event {
    friend class Physics;
    friend class EventValidation;
    private:
      int m_PhysicsID = -1;
      bool m_Collides = false;
      std::vector<unsigned int> m_CollidesWith;
    public:
      PhysicsCollisionCheckGenericEvent(const unsigned int physicsID) : m_PhysicsID(physicsID) {
        this->SetType(EventType::PHYSICS_COLLISION_CHECK_GENERIC);
      }
      bool Collides() const {return m_Collides;}
      // Returns a vector containing the physicsIDs that the given physicsID collides with
      std::vector<unsigned int> CollidesWith() const {return m_CollidesWith;}
  };

}

#endif