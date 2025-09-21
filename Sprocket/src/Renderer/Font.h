#ifndef FONT_H
#define FONT_H

#include "Core/Sprocket.pch"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

    struct Character {
        glm::vec2 uv0; // Top left uv
        glm::vec2 uv1; // Botton right uv
        unsigned int width;
        unsigned int height;
        glm::vec2 bearing;
        int advance;
    };

    struct CharacterAtlas{
        unsigned int width;
        unsigned int height;
        std::vector<unsigned char> pixels;
    };

    class Font {
        friend class Renderer;
    private:
       
        std::string m_FontPath;
        std::map<char, Character> m_Characters;
        CharacterAtlas m_Atlas;

    public:

        /// @brief Creates a font object based on the given path and font size.
        /// @param fontPath - The path to the font file.
        /// @param fontSizeInPixels - The size the font show be created for.
        Font(const std::string& fontPath, const unsigned int fontSizeInPixels);

        /// @brief Generates a texture buffer based on the given text.
        /// @param text - The text to generate a buffer for.
        /// @param width - The width of the generated buffer.
        /// @param height - The height of the generated buffer.
        /// @return A buffer that can be used to render the text as a single texture.
        std::vector<unsigned char> GetTextureBufferForText(const std::string& text, unsigned int& width, unsigned int& height);

    };

}

#endif