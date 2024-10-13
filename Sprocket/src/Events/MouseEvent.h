#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include "Event.h"
#include "Core/MouseButtons.h"

namespace Sprocket {

  class MouseButtonEvent : public Event {
    private:
      MouseButton m_MouseButton;
    protected:
      MouseButtonEvent(const MouseButton mouseButton) : m_MouseButton(mouseButton) {}
    public:
      MouseButton GetMouseButton() const {return m_MouseButton;}
  };

  class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
      MouseButtonPressedEvent(const MouseButton mouseButton) : MouseButtonEvent(mouseButton) {
        this->SetType(EventType::MOUSE_BTN_PRESSED);
      }
  };

  class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
      MouseButtonReleasedEvent(const MouseButton mouseButton) : MouseButtonEvent(mouseButton) {
        this->SetType(EventType::MOUSE_BTN_RELEASED);
      }
  };

  class MouseMovedEvent : public Event {
    private:
      double m_XPos;
      double m_YPos;
    public:
      MouseMovedEvent(const double xPos, const double yPos) : m_XPos(xPos), m_YPos(yPos) {
        this->SetType(EventType::MOUSE_MOVED);
      }
      double GetXPosition() const {return m_XPos;}
      double GetYPosition() const {return m_YPos;}
  };

  class MouseScrolledEvent : public Event {
    private:
      double m_XOffset;
      double m_YOffset;
    public:
      MouseScrolledEvent(const double xOffset, const double yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {
        this->SetType(EventType::MOUSE_SCROLLED);
      }
      double GetXOffset() const {return m_XOffset;}
      double GetYOffset() const {return m_YOffset;}
  };

}

#endif