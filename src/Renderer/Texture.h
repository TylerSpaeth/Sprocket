#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
  private:
    unsigned int textureID;
    std::string filePath;
    unsigned char* localBuffer;
    int width, height, bitsPerPixel;
  public:
    Texture(const std::string& path, unsigned int slot);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const {return width;}
    inline int GetHeight() const {return height;}
};

#endif