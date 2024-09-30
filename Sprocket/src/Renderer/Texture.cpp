#include "Texture.h"
#include "ThirdParty/glad/glad.h"
#include "ThirdParty/stb_image/stb_image.h"

#include <iostream>

// Binds the texture specified in the path to the given slot
Texture::Texture(const std::string& path, unsigned int slot) 
    : filePath(path), localBuffer(nullptr), width(0), height(0), bitsPerPixel(0) {  

  // OpenGL sets 0,0 to bottom left while nomally it would be top left
  stbi_set_flip_vertically_on_load(1);
  localBuffer = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 4);
  
  glGenTextures(1, &textureID);
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);

  // If there is a loaded image, free it
  if(localBuffer) {
    stbi_image_free(localBuffer);
  }
  else if(localBuffer == nullptr) {
    std::cout << "Failed to load texture\n";
  }
}

Texture::~Texture() {
  glDeleteTextures(1, &textureID);
}

void Texture::Bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}