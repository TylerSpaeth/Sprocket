#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

#include "Event.h"
#include "Core/Keycodes.h"

namespace Sprocket {

    class KeyEvent : public Event {
    private:
        Keycode m_Keycode;
    public:
        Keycode GetKeycode() const { return m_Keycode; }
    protected:
        KeyEvent(const Keycode keycode) : m_Keycode(keycode) {}
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(const Keycode keycode) : KeyEvent(keycode) {
            this->SetType(EventType::KEY_PRESSED);
        }
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(const Keycode keycode) : KeyEvent(keycode) {
            this->SetType(EventType::KEY_RELEASED);
        }
    };

};

#endif