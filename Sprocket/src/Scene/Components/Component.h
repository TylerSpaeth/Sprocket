#ifndef COMPONENT_H
#define COMPONENT_H

#include "Scene/OnActivateParams.h"
#include "Scene/OnDeactivateParams.h"
#include "Scene/OnUpdateParams.h"

#include "Core/Macros.h"
#include "Core/Sprocket.pch"

#include "Events/Event.h"

#include "Utils/UUID.h"

namespace Sprocket {

    /// @brief The base class for all Component.
    class SPROCKET_API Component {

        friend class Entity;

    public:

        const uint64_t m_UUID = UUID::Generate();

    public:
        // This is needed to allow dynamic cast checking against Component pointers
        virtual ~Component() = default;

        uint64_t GetUUID() const { return m_UUID; }

    protected:

        virtual void OnActivate(OnActivateParams& onActivateParams) = 0;
        virtual void OnDeactivate(OnDeactivateParams& onDeactivateParams) = 0;
        virtual void OnUpdate(OnUpdateParams& onUpdateParams) = 0;

    };
    
    /// @brief An extension of the base Component for components that need to be able to send events.
    class EventDrivenComponent : public Component {
    protected:
        // A callback that sends events into the event system.
        std::function<void(Event&)> m_EventCallback;
    public:
        virtual void RegisterEventCallback(std::function<void(Event&)> eventCallback) {m_EventCallback = eventCallback;}
    };

}

#endif