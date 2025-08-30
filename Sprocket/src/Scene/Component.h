#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Macros.h"
#include "Core/Sprocket.pch"

#include "Events/Event.h"

namespace Sprocket {

    class SPROCKET_API Component {

    public:
        // This is needed to allow dynamic cast checking against Component pointers
        virtual ~Component() = default;

    };

    class EventDrivenComponent : public Component {
    protected:
        std::function<void(Event&)> m_EventCallback;
    };

}

#endif