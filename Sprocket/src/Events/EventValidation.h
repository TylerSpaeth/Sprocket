#ifndef EVENTVALIDATION_H
#define EVENTVALIDATION_H

#include "Event.h"
#include "ApplicationEvent.h"
#include "RenderEvent.h"
#include "PhysicsEvent.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

    // EventValidation is used to verify that a given event is valid. If validation fails in one of 
    // these functions, then exceptions will be thrown, since this validation is intended to be a
    // last line of defense against improper/failed validation in event producers.
    class EventValidation {

    public:

        static void ValidateEvent(const Event& event);

    private:

        static void ValidateAppUpdateEvent(const ApplicationUpdateEvent& event);
        static void ValidateRenderNewEvent(const RenderNewEvent& event);
        static void ValidateRenderUpdateEvent(const RenderUpdateEvent& event);
        static void ValidateRenderDeleteEvent(const RenderDeleteEvent& event);
        static void ValidatePhysicsNewEvent(const PhysicsNewEvent& event);
        static void ValidatePhysicsUpdateEvent(const PhysicsUpdateEvent& event);
        static void ValidatePhysicsDeleteEvent(const PhysicsDeleteEvent& event);
        static void ValidatePhysicsCollisionCheckEvent(const PhysicsCollisionCheckEvent& event);
        static void ValidatePhysicsCollisionCheckGenericEvent(const PhysicsCollisionCheckGenericEvent& event);

    };

}

#endif