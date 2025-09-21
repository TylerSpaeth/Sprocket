#include "EventValidation.h"

#include "Core/Global.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void EventValidation::ValidateEvent(const Event& event) {
        switch (event.GetEventType()) {
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
        case EventType::AUDIO_NEW:
            ValidateAudioNewEvent((AudioNewEvent&)event);
            break;
        case EventType::AUDIO_DELETE:
            ValidateAudioDeleteEvent((AudioDeleteEvent&)event);
            break;
        case EventType::AUDIO_SETTER:
            ValidateAudioSetterEvent((AudioSetterEvent&)event);
            break;
        case EventType::AUDIO_GETTER:
            ValidateAudioGetterEvent((AudioGetterEvent&)event);
            break;
        case EventType::AUDIO_ACTION:
            ValidateAudioActionEvent((AudioActionEvent&)event);
            break;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void EventValidation::ValidateAppUpdateEvent(const ApplicationUpdateEvent& event) {
        if (event.GetDeltaTime() < 0) {
            Global::fileLogger.Error(std::format("ApplicationUpdateEvent failed, delta time is negative: {}", event.GetDeltaTime()));
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidateRenderNewEvent(const RenderNewEvent& event) {
        if (event.GetQuadID() != -1) {
            Global::fileLogger.Error("RenderNewEvent failed, this event has already been used.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidateRenderUpdateEvent(const RenderUpdateEvent& event) {
        if (event.GetType() != RenderUpdateType::VIEW_MATRIX && event.GetQuadID() < 0) {
            Global::fileLogger.Error("RenderUpdateEvent failed, invalid quadID.");
            std::exit(EXIT_FAILURE);
        }
        // No other validation will be done here since the rest of the fields have default values
        // that are valid
    }

    void EventValidation::ValidateRenderDeleteEvent(const RenderDeleteEvent& event) {
        if (event.GetQuadID() < 0) {
            Global::fileLogger.Error("RenderDeleteEvent failed, invalid quadID.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidatePhysicsNewEvent(const PhysicsNewEvent& event) {
        if (event.m_PhysicsID != -1) {
            Global::fileLogger.Error("PhysicsNewEvent failed, this event has already been used.");
            std::exit(EXIT_FAILURE);
        }
        if (event.m_CircleRadius < 0) {
            Global::fileLogger.Error("PhysicsNewEvent failed, circleRadius is negative.");
            std::exit(EXIT_FAILURE);
        }
        if (event.m_BoxColliderSize.x < 0 || event.m_BoxColliderSize.y < 0) {
            Global::fileLogger.Error("PhysicsNewEvent failed, boxColliderSize has a negative dimension.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidatePhysicsUpdateEvent(const PhysicsUpdateEvent& event) {
        if (event.m_PhysicsID < 0) {
            Global::fileLogger.Error("PhysicsUpdateEvent failed, invalid physicsID.");
            std::exit(EXIT_FAILURE);
        }
        if (event.m_CircleRadius < 0) {
            Global::fileLogger.Error("PhysicsUpdateEvent failed, circleRadius is negative.");
            std::exit(EXIT_FAILURE);
        }
        if (event.m_BoxColliderSize.x < 0 || event.m_BoxColliderSize.y < 0) {
            Global::fileLogger.Error("PhysicsUpdateEvent failed, boxColliderSize has a negative dimension.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidatePhysicsDeleteEvent(const PhysicsDeleteEvent& event) {
        if (event.m_PhysicsID < 0) {
            Global::fileLogger.Error("PhysicsDeleteEvent failed, invalid physicsID.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidatePhysicsCollisionCheckEvent(const PhysicsCollisionCheckEvent& event) {
        if (event.m_PhysicsID1 < 0 || event.m_PhysicsID2 < 0) {
            Global::fileLogger.Error("PhysicsCollisionCheckEvent failed, invalid physicsID.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidatePhysicsCollisionCheckGenericEvent(const PhysicsCollisionCheckGenericEvent& event) {
        if (event.m_PhysicsID < 0) {
            Global::fileLogger.Error("PhysicsCollisionCheckGenericEvent failed, invalid physicsID.");
            std::exit(EXIT_FAILURE);
        }
        if (!event.m_CollidesWith.empty()) {
            Global::fileLogger.Error("PhysicsCollisionCheckGenericEvent failed, collidesWith must be empty.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidateAudioNewEvent(const AudioNewEvent& event) {
        if (event.GetFilepath().empty()) {
            Global::fileLogger.Error("AudioNewEvent failed, filepath is empty.");
            std::exit(EXIT_FAILURE);
        }
        if (event.GetSoundID() != -1) {
            Global::fileLogger.Error("AudioNewEvent failed, this event has already been used.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidateAudioDeleteEvent(const AudioDeleteEvent& event) {
        if (event.GetSoundID() < 0) {
            Global::fileLogger.Error("AudioDeleteEvent failed, invalid soundID.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidateAudioSetterEvent(const AudioSetterEvent& event) {
        if (event.GetSoundID() < 0) {
            Global::fileLogger.Error("AudioSetterEvent failed, invalid soundID.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidateAudioGetterEvent(const AudioGetterEvent& event) {
        if (event.GetSoundID() < 0) {
            Global::fileLogger.Error("AudioGetterEvent failed, invalid soundID.");
            std::exit(EXIT_FAILURE);
        }
    }

    void EventValidation::ValidateAudioActionEvent(const AudioActionEvent& event) {
        if (event.GetSoundID() < 0) {
            Global::fileLogger.Error("AudioActionEvent failed, invalid soundID.");
            std::exit(EXIT_FAILURE);
        }
    }

}