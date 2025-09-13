#ifndef TEXTURE_H
#define TEXTURE_H

#include "Core/Sprocket.pch"

namespace Sprocket {

    // TODO the texture system needs reworking to be more flexible

    class Texture {
        friend class Renderer;
    private:
        unsigned int m_TextureID;
        std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BitsPerPixel;
        mutable unsigned int m_Slot = -1;

        /// @brief Called by the constructors after data initialization
        void CreateTexture();
    public:

        /// @brief Constructs a new texture based on the given image data buffer
        /// @param buffer a buffer containing image data
        Texture(unsigned char* buffer);

        /// @brief Constructs a new texture based on the given path.
        /// @param path the filepath to the texture.
        Texture(const std::string& path);

        /// @brief Removes this texture from OpenGL's textures.
        ~Texture();

        /// @brief Binds this texture to the given texture slot.
        /// @param slot the texture slot to bind this texture to.
        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }
    };

}

#endif