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
        const float GetDeltaTime() const { return m_DeltaTime; }
    };

    /// @brief The first event the occurs in the event system. Contains all of the application
    /// metadata needed for startup.
    class ApplicationStartEvent : public ApplicationEvent {
    private:
        float m_WindowXDimension;
        float m_WindowYDimension;
    public:
        ApplicationStartEvent(float windowXDimension, float windowYDimension) : 
            m_WindowXDimension(windowXDimension), m_WindowYDimension(windowYDimension) {
            this->SetType(EventType::APP_START);
        }
        const float GetWindowXDimension() const {return m_WindowXDimension;}
        const float GetWindowYDimension() const {return m_WindowYDimension;}
    };

    class ApplicationShutdownEvent : public ApplicationEvent {
    public:
        ApplicationShutdownEvent() {
            this->SetType(EventType::APP_SHUTDOWN);
        }
    };

}

#endif
