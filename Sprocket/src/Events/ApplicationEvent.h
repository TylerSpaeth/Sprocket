#ifndef APPLICATIONEVENT_H
#define APPLICATIONEVENT_H

#include "Event.h"

namespace Sprocket {

  class ApplicationEvent : public Event {
    protected:
      ApplicationEvent(){}
  };

  class ApplicationUpdateEvent : public ApplicationEvent {
    private:
      float m_DeltaTime; // Time since last update
    public:
      ApplicationUpdateEvent(float deltaTime) : m_DeltaTime(deltaTime) {
        this->SetType(APP_UPDATE);
      }
      float GetDeltaTime() const {return m_DeltaTime;}
  };

}

#endif
