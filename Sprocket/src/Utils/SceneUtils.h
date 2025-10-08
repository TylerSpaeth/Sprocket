#ifndef SCENEUTILS_H
#define SCENEUTILS_h

#include "Core/Sprocket.pch"
#include "Core/Macros.h"

#include "Scene/Components/Subcomponents/Sprite.h"

namespace Sprocket {

    /// @brief Turns a spritesheet into a vector of sprites from left to right, top to bottom
    /// @param spriteSheet The path to the spritesheet texture
    /// @param sheetWidth The width of the entire spritesheet in pixels
    /// @param sheetHeight The height of the entire spritesheet in pixels
    /// @param spriteWidth The width of each individual sprite in pixels
    /// @param spriteHeight The height of each individual sprite in pixels
    /// @return A vector of sprites
    std::vector<Sprite> SPROCKET_API GetSpritesFromSpritesheet(const std::string& spriteSheet, int sheetWidth, int sheetHeight, int spriteWidth, int spriteHeight);

}

#endif