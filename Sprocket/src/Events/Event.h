#ifndef EVENT_H
#define EVENT_H

namespace Sprocket {

  enum class EventType {
    NONE,
    // KEYBOARD
    KEY_PRESSED, KEY_RELEASED,
    // MOUSE
    MOUSE_BTN_PRESSED, MOUSE_BTN_RELEASED, MOUSE_MOVED, MOUSE_SCROLLED,
    // WINDOW
    WINDOW_CLOSE,
    // APPLICATION
    APP_UPDATE,
    // RENDER
    RENDER_NEW, RENDER_UPDATE
  };

  // For future use
  enum class EventCategory {
    UNCATEGORIZED, // Every event type can be classified as uncategorized
    APPLICATION,
    WINDOW,
    INPUT,
    KEYBOARD,
    MOUSE,
    RENDER,
  };

  class Event {
    private:
      EventType m_EventType = EventType::NONE;
    public:
      void SetType(EventType type) {
        m_EventType = type;
      }

      EventType GetEventType() const {return m_EventType;}
      
      // For future use
      bool IsCategory(EventCategory eventCategory) {
        if(eventCategory == EventCategory::UNCATEGORIZED) {
          return true;
        }
        switch(m_EventType) {
          case EventType::KEY_PRESSED:
          case EventType::KEY_RELEASED:
            return eventCategory == EventCategory::INPUT || eventCategory == EventCategory::KEYBOARD || eventCategory == EventCategory::APPLICATION;
          case EventType::MOUSE_BTN_PRESSED:
          case EventType::MOUSE_BTN_RELEASED:
          case EventType::MOUSE_MOVED:
          case EventType::MOUSE_SCROLLED:
            return eventCategory == EventCategory::INPUT || eventCategory == EventCategory::MOUSE || eventCategory == EventCategory::APPLICATION;
          case EventType::WINDOW_CLOSE:
            return eventCategory == EventCategory::APPLICATION || eventCategory == EventCategory::WINDOW;
          case EventType::APP_UPDATE:
            return eventCategory == EventCategory::APPLICATION;
          case EventType::RENDER_NEW:
          case EventType::RENDER_UPDATE:
            return eventCategory == EventCategory::RENDER;
        }

        return false;
      }
  };

}

#endif