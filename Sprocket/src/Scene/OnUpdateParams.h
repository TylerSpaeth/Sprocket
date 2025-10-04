#ifndef ONUPDATEPARAMS_H
#define ONUPDATEPARAMS_H

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

    struct OnUpdateParams {
        float deltaTime;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

}

#endif