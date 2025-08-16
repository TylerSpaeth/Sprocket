#include "EventValidation.h"

#include <stdexcept>

namespace Sprocket {

  void EventValidation::ValidateEvent(const Event& event) {
    switch(event.GetEventType()) {
      case EventType::APP_UPDATE:
        ValidateAppUpdateEvent((ApplicationUpdateEvent&)event);
        break;
      case EventType::RENDER_NEW:
        ValidateRenderNewEvent((RenderNewEvent&)event);
        break;
      case EventType::RENDER_UPDATE:
        ValidateRenderUpdateEvent((RenderUpdateEvent&)event);
        break;
      case EventType::RENDER_DELETE:
        ValidateRenderDeleteEvent((RenderDeleteEvent&)event);
        break;
      case EventType::PHYSICS_NEW:
        ValidatePhysicsNewEvent((PhysicsNewEvent&)event);
        break;
      case EventType::PHYSICS_UPDATE:
        ValidatePhysicsUpdateEvent((PhysicsUpdateEvent&)event);
        break;
      case EventType::PHYSICS_DELETE:
        ValidatePhysicsDeleteEvent((PhysicsDeleteEvent&)event);
        break;
      case EventType::PHYSICS_COLLISION_CHECK:
        ValidatePhysicsCollisionCheckEvent((PhysicsCollisionCheckEvent&)event);
        break;
      case EventType::PHYSICS_COLLISION_CHECK_GENERIC:
        ValidatePhysicsCollisionCheckGenericEvent((PhysicsCollisionCheckGenericEvent&)event);
        break;
    }
  }

  void EventValidation::ValidateAppUpdateEvent(const ApplicationUpdateEvent& event) {
    if(event.GetDeltaTime() < 0) {
      throw std::invalid_argument("deltaTime can not be less than 0.");
    }
  }

  void EventValidation::ValidateRenderNewEvent(const RenderNewEvent& event) {
    if(event.GetQuadID() != -1) {
      throw std::invalid_argument("This event has already be used.");
    }
  }

  void EventValidation::ValidateRenderUpdateEvent(const RenderUpdateEvent& event) {
    if(event.GetType() != RenderUpdateType::VIEW_MATRIX && event.GetQuadID() < 0) {
      throw std::invalid_argument("Invalid quadID.");
    }
    // No other validation will be done here since the rest of the fields have default values
    // that are valid
  }

  void EventValidation::ValidateRenderDeleteEvent(const RenderDeleteEvent& event) {
    if(event.GetQuadID() < 0) {
      throw std::invalid_argument("Invalid quadID.");
    }
  }

  void EventValidation::ValidatePhysicsNewEvent(const PhysicsNewEvent& event) {
    if(event.m_PhysicsID != -1) {
      throw std::invalid_argument("This event has already been used.");
    }
    if(event.m_CircleRadius < 0) {
      throw std::invalid_argument("circleRadius is negative.");
    }
    if(event.m_BoxColliderSize.x < 0 || event.m_BoxColliderSize.y < 0) {
      throw std::invalid_argument("boxColliderSize has a negative dimension.");
    }
  }

  void EventValidation::ValidatePhysicsUpdateEvent(const PhysicsUpdateEvent& event) {
    if(event.m_PhysicsID < 0) {
      throw std::invalid_argument("Invalid physicsID.");
    }
    if(event.m_CircleRadius < 0) {
      throw std::invalid_argument("circleRadius is negative.");
    }
    if(event.m_BoxColliderSize.x < 0 || event.m_BoxColliderSize.y < 0) {
      throw std::invalid_argument("boxColliderSize has a negative dimension.");
    }
  }

  void EventValidation::ValidatePhysicsDeleteEvent(const PhysicsDeleteEvent& event) {
    if(event.m_PhysicsID < 0) {
      throw std::invalid_argument("Invalid physicsID.");
    }
  }

  void EventValidation::ValidatePhysicsCollisionCheckEvent(const PhysicsCollisionCheckEvent& event) {
    if(event.m_PhysicsID1 < 0 || event.m_PhysicsID2 < 0) {
      throw std::invalid_argument("Invalid physicsID.");
    }
  }

  void EventValidation::ValidatePhysicsCollisionCheckGenericEvent(const PhysicsCollisionCheckGenericEvent& event) {
    if(event.m_PhysicsID < 0) {
      throw std::invalid_argument("Invalid physicsID.");
    }
    if(!event.m_CollidesWith.empty()) {
      throw std::invalid_argument("This event has already been used.");
    }
  }

}