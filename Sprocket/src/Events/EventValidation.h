#ifndef EVENTVALIDATION_H
#define EVENTVALIDATION_H

#include "Event.h"
#include "ApplicationEvent.h"
#include "RenderEvent.h"
#include "PhysicsEvent.h"
#include "AudioEvent.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

    // EventValidation is used to verify that a given event is valid. If validation fails in one of 
    // these functions, then an error will be logged and the application will be terminated. It is 
    // up to other systems to ensure that they are passing in valid events. If they fail to do so, 
    // this is the last line of defense and obviosuly shoudl not be relied on for normal operation.
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
        static void ValidateAudioNewEvent(const AudioNewEvent& event);
        static void ValidateAudioDeleteEvent(const AudioDeleteEvent& event);
        static void ValidateAudioSetterEvent(const AudioSetterEvent& event);
        static void ValidateAudioGetterEvent(const AudioGetterEvent& event);
        static void ValidateAudioActionEvent(const AudioActionEvent& event);

    };

}

#endif