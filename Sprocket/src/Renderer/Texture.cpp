#include "Texture.h"
#include "ThirdParty/glad/glad.h"
#include "ThirdParty/stb_image/stb_image.h"
#include "Core/Global.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    Texture::Texture(unsigned char* buffer, unsigned int width, unsigned int height, unsigned int bytesPerPixel) 
            : m_FilePath(""), m_Width(width), m_Height(height), m_BytesPerPixel(bytesPerPixel) {
        size_t size = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(bytesPerPixel);
        m_LocalBuffer = new unsigned char[size];
        memcpy(m_LocalBuffer, buffer, size);
        CreateTexture();
    }

    // Binds the texture specified in the path to the given slot
    Texture::Texture(const std::string& path)
        : m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BytesPerPixel(0) {

        // OpenGL sets 0,0 to bottom left while nomally it would be top left
        stbi_set_flip_vertically_on_load(1);
        m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BytesPerPixel, 4);

        if (m_LocalBuffer == nullptr) {
            Global::FileLogger().Error(std::format("Sprocket: Texture loading failed for path: {}", path));
        }

        CreateTexture();

        // If there is a loaded image, free it
        if (m_LocalBuffer) {
            stbi_image_free(m_LocalBuffer);
        }

    }

    Texture::~Texture() {
        glMakeTextureHandleNonResidentARB(m_Handle);
        glDeleteTextures(1, &m_TextureID);
    }

    const int Texture::GetWidth() const {
        return m_Width;
    }

    const int Texture::GetHeight() const {
        return m_Height;
    }

    const unsigned long long Texture::GetHandle() const {
        return m_Handle;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Texture::CreateTexture() {
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);

        // Get bindless handle
        m_Handle = glGetTextureHandleARB(m_TextureID);
        glMakeTextureHandleResidentARB(m_Handle);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

}