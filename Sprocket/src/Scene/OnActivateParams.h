#ifndef ONACTIVATEPARAMS_H
#define ONACTIVATEPARAMS_H

#include "Core/Sprocket.pch"

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

    /// @brief Struct containing parameters for calling OnActivate for all Components in an Entity
    struct OnActivateParams {

        std::function<void(Event&)> eventCallback;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

    };

}

#endif