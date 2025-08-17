#include "Texture.h"
#include "ThirdParty/glad/glad.h"
#include "ThirdParty/stb_image/stb_image.h"

namespace Sprocket {

    // Binds the texture specified in the path to the given slot
    Texture::Texture(const std::string& path)
        : m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BitsPerPixel(0) {

        // OpenGL sets 0,0 to bottom left while nomally it would be top left
        stbi_set_flip_vertically_on_load(1);
        m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);

        glGenTextures(1, &m_TextureID);
        // FIXME for some reason this needs to be called in order to be able to render multiple textures at the same time. This should be done a different way because there is no guarentee that the textureID and slot will always be the same
        glActiveTexture(GL_TEXTURE0 + m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);

        // If there is a loaded image, free it
        if (m_LocalBuffer) {
            stbi_image_free(m_LocalBuffer);
        }
        else if (m_LocalBuffer == nullptr) {
            std::println("Failed to load texture");
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_TextureID);
    }

    void Texture::Bind(unsigned int slot) const {
        m_Slot = slot;
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void Texture::Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}