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
      Texture(const std::string& path);
      ~Texture();

      void Bind(unsigned int slot = 0) const;
      void Unbind() const;

      inline int GetWidth() const {return width;}
      inline int GetHeight() const {return height;}
  };

}

#endif