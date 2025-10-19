#include "SceneUtils.h"

#include "Core/Global.h"

namespace Sprocket {
    
    std::vector<Sprite> GetSpritesFromSpritesheet(const std::string& spriteSheet, int sheetWidth, 
        int sheetHeight, int spriteWidth, int spriteHeight) {

        std::vector<Sprite> sprites;

        if (sheetWidth % spriteWidth != 0 || sheetHeight % spriteHeight != 0) {
            Global::FileLogger().Warning("GetSpritesFromSpriteSheet: Sheet dimensions must be divisible by sprite dimensions");
            return sprites;
        }

        int spritesPerRow = sheetWidth / spriteWidth;
        int spritesPerColumn = sheetHeight / spriteHeight;

        for (int i = 0; i < spritesPerColumn; i++) {
            for (int j = 0; j < spritesPerRow; j++) {
                Sprite sprite;
                sprite.texturePath = spriteSheet;
                sprite.textureXUVCoords = {
                                          (float) ((j+1) * spriteWidth - 1) / sheetWidth,
                                          (float) ((j+1) * spriteWidth - 1) / sheetWidth,
                                          (float) (j * spriteWidth) / sheetWidth,
                                          (float) (j * spriteWidth) / sheetWidth
                                          };
                sprite.textureYUVCoords = {
                                          (float) ((i+1) * spriteHeight - 1) / sheetHeight,
                                          (float) (i * spriteHeight) / sheetHeight,
                                          (float) (i * spriteHeight) / sheetHeight,
                                          (float) ((i+1) * spriteHeight - 1) / sheetHeight
                                          };
                sprites.push_back(sprite);
            }
        }

        return sprites;

    }

}