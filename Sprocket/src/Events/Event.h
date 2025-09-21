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
        RENDER_NEW, RENDER_UPDATE, RENDER_DELETE, RENDER_NEW_TEXT,
        // PHYSICS
        PHYSICS_NEW, PHYSICS_UPDATE, PHYSICS_DELETE, PHYSICS_COLLISION_CHECK, PHYSICS_COLLISION_CHECK_GENERIC,
        // AUDIO
        AUDIO_NEW, AUDIO_DELETE, AUDIO_SETTER, AUDIO_GETTER, AUDIO_ACTION
    };

    // For future use
    enum class EventCategory {
        UNCATEGORIZED, // Every event type can be classified as uncategorized
        APPLICATION,
        INPUT,
        KEYBOARD,
        MOUSE,
        RENDER,
        PHYSICS,
        AUDIO
    };

    class Event {

    private:
        EventType m_EventType = EventType::NONE;

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
            case EventType::RENDER_NEW_TEXT:
                return eventCategory == EventCategory::RENDER;
            case EventType::PHYSICS_NEW:
            case EventType::PHYSICS_UPDATE:
            case EventType::PHYSICS_DELETE:
            case EventType::PHYSICS_COLLISION_CHECK:
            case EventType::PHYSICS_COLLISION_CHECK_GENERIC:
                return eventCategory == EventCategory::PHYSICS;
            case EventType::AUDIO_NEW:
            case EventType::AUDIO_DELETE:
            case EventType::AUDIO_SETTER:
            case EventType::AUDIO_GETTER:
            case EventType::AUDIO_ACTION:
                return eventCategory == EventCategory::AUDIO;
            }

            return false;
        }

    protected:
        void SetType(EventType type) {
            m_EventType = type;
        }
    };

}

#endif