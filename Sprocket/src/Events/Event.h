#ifndef EVENT_H
#define EVENT_H

namespace Sprocket {

    enum class EventType {
        NONE,
        // KEYBOARD
        KEY_PRESSED, KEY_RELEASED,
        // MOUSE
        MOUSE_BTN_PRESSED, MOUSE_BTN_RELEASED, MOUSE_MOVED, MOUSE_SCROLLED,
        // APPLICATION
        APP_UPDATE, APP_START, APP_SHUTDOWN,
        // RENDER
        RENDER_NEW, RENDER_UPDATE, RENDER_DELETE,
        // PHYSICS
        PHYSICS_NEW, PHYSICS_UPDATE, PHYSICS_DELETE, PHYSICS_COLLISION_CHECK, PHYSICS_COLLISION_CHECK_GENERIC
    };

    // For future use
    enum class EventCategory {
        UNCATEGORIZED, // Every event type can be classified as uncategorized
        APPLICATION,
        INPUT,
        KEYBOARD,
        MOUSE,
        RENDER,
        PHYSICS
    };

    class Event {

    private:
        EventType m_EventType = EventType::NONE;

    protected:
        void SetType(EventType type) {
            m_EventType = type;
        }

    public:

        EventType GetEventType() const { return m_EventType; }

        bool IsCategory(EventCategory eventCategory) {
            if (eventCategory == EventCategory::UNCATEGORIZED) {
                return true;
            }
            switch (m_EventType) {
            case EventType::KEY_PRESSED:
            case EventType::KEY_RELEASED:
                return eventCategory == EventCategory::INPUT || eventCategory == EventCategory::KEYBOARD || eventCategory == EventCategory::APPLICATION;
            case EventType::MOUSE_BTN_PRESSED:
            case EventType::MOUSE_BTN_RELEASED:
            case EventType::MOUSE_MOVED:
            case EventType::MOUSE_SCROLLED:
                return eventCategory == EventCategory::INPUT || eventCategory == EventCategory::MOUSE || eventCategory == EventCategory::APPLICATION;
            case EventType::APP_UPDATE:
            case EventType::APP_START:
            case EventType::APP_SHUTDOWN:
                return eventCategory == EventCategory::APPLICATION;
            case EventType::RENDER_NEW:
            case EventType::RENDER_UPDATE:
            case EventType::RENDER_DELETE:
                return eventCategory == EventCategory::RENDER;
            case EventType::PHYSICS_NEW:
            case EventType::PHYSICS_UPDATE:
            case EventType::PHYSICS_DELETE:
            case EventType::PHYSICS_COLLISION_CHECK:
            case EventType::PHYSICS_COLLISION_CHECK_GENERIC:
                return eventCategory == EventCategory::PHYSICS;
            }

            return false;
        }
    };

}

#endif