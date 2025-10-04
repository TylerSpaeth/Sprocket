#ifndef ANIMATION_H
#define ANIMATION_H

#include "Sprite.h"

#include "Core/Sprocket.pch"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

    struct AnimationFrame {
        Sprite sprite;
        glm::vec4 color = {1,1,1,1};
        // Frame duration is in seconds
        float frameDuration;
    };

    struct Animation {

        std::vector<AnimationFrame> animationFrames;

    };

}

#endif