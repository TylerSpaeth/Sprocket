#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Macros.h"
#include "Core/Sprocket.pch"

#include "Events/Event.h"

namespace Sprocket {

    /// @brief The base class for all Component.
    class SPROCKET_API Component {

    public:
        // This is needed to allow dynamic cast checking against Component pointers
        virtual ~Component() = default;

    };
    
    /// @brief An extension of the base Component for components that need to be able to send events.
    class EventDrivenComponent : public Component {
    protected:
        // A callback that sends events into the event system.
        std::function<void(Event&)> m_EventCallback;
    };

}

#endif