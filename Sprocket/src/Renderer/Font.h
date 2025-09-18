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
        unsigned int m_TextureID;

    public:

        Font(const std::string& fontPath, const unsigned int fontSizeInPixels);

        std::map<char, Character> GetCharacters() {return m_Characters;}

        unsigned int GetTextureID() const {return m_TextureID;}

    };

}

#endif