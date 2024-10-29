#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

namespace Sprocket {

  // TODO the texture system needs reworking to be more flexible

  class Texture {
    friend class Renderer;
    private:
      unsigned int textureID;
      std::string filePath;
      unsigned char* localBuffer;
      int width, height, bitsPerPixel;
      mutable unsigned int m_Slot = -1;
    public:

      /// @brief Constructs a new texture based on the given path.
      /// @param path the filepath to the texture.
      Texture(const std::string& path);

      /// @brief Removes this texture from OpenGL's textures.
      ~Texture();

      /// @brief Binds this texture to the given texture slot.
      /// @param slot the texture slot to bind this texture to.
      void Bind(unsigned int slot = 0) const;
      void Unbind() const;

      inline int GetWidth() const {return width;}
      inline int GetHeight() const {return height;}
  };

}

#endif