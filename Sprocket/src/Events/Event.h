#ifndef EVENT_H
#define EVENT_H

namespace Sprocket {

  enum EventType {
    NONE,
    // KEYBOARD
    KEY_PRESSED, KEY_RELEASED,
    // MOUSE
    MOUSE_BTN_PRESSED, MOUSE_BTN_RELEASED, MOUSE_MOVED, MOUSE_SCROLLED,
    // WINDOW
    WINDOW_CLOSE,
    // APPLICATION
    APP_UPDATE
  };

  enum EventCategory {
    UNCATEGORIZED,
    APPLICATION,
    WINDOW,
    INPUT,
    KEYBOARD,
    MOUSE
  };

  class Event {
    private:
      EventType m_EventType = NONE;
    public:
      void SetType(EventType type) {
        m_EventType = type;
      }

      EventType GetEventType() const {return m_EventType;}
      
      bool IsCategory(EventCategory eventCategory) {
        switch(m_EventType) {
          case EventType::KEY_PRESSED:
          case EventType::KEY_RELEASED:
            return eventCategory == EventCategory::INPUT || eventCategory == EventCategory::KEYBOARD;
          case EventType::MOUSE_BTN_PRESSED:
          case EventType::MOUSE_BTN_RELEASED:
          case EventType::MOUSE_MOVED:
          case EventType::MOUSE_SCROLLED:
            return eventCategory == EventCategory::INPUT || eventCategory == EventCategory::MOUSE;
          case EventType::WINDOW_CLOSE:
            return eventCategory == EventCategory::APPLICATION || eventCategory == EventCategory::WINDOW;
          case EventType::APP_UPDATE:
            return eventCategory == EventCategory::APPLICATION;
        }
      }
  };

}

#endif