#ifndef APPLICATIONEVENT_H
#define APPLICATIONEVENT_H

#include "Event.h"

namespace Sprocket {

    class ApplicationEvent : public Event {
    protected:
        ApplicationEvent() {}
    };

    class ApplicationUpdateEvent : public ApplicationEvent {
    private:
        float m_DeltaTime; // Time since last update
    public:
        ApplicationUpdateEvent(float deltaTime) : m_DeltaTime(deltaTime) {
            this->SetType(EventType::APP_UPDATE);
        }
        float GetDeltaTime() const { return m_DeltaTime; }
    };

    class ApplicationStartEvent : public ApplicationEvent {
    public:
        ApplicationStartEvent() {
            this->SetType(EventType::APP_START);
        }
    };

    class ApplicationShutdownEvent : public ApplicationEvent {
    public:
        ApplicationShutdownEvent() {
            this->SetType(EventType::APP_SHUTDOWN);
        }
    };

}

#endif
