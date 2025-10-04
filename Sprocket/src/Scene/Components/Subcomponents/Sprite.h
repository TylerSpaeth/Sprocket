#ifndef SPRITE_H
#define SPRITE_H

#include "Core/Sprocket.pch"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket{
    
    struct Sprite {
        std::string texturePath;
        // These coordinates start in the top right corner and go clockwise around the quad
        glm::vec4 textureXUVCoords = { 1,1,0,0 };
        glm::vec4 textureYUVCoords = { 1,0,0,1 };
        glm::vec4 color = { 1,1,1,1 };
    };

}

#endif