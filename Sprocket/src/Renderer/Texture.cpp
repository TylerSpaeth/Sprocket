#include "Texture.h"
#include "ThirdParty/glad/glad.h"
#include "ThirdParty/stb_image/stb_image.h"

#include <print>

namespace Sprocket {

  // Binds the texture specified in the path to the given slot
  Texture::Texture(const std::string& path) 
      : filePath(path), localBuffer(nullptr), width(0), height(0), bitsPerPixel(0) {  

    // OpenGL sets 0,0 to bottom left while nomally it would be top left
    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 4);
    
    glGenTextures(1, &textureID);
    // FIXME for some reason this needs to be called in order to be able to render multiple textures at the same time. This should be done a different way because there is no guarentee that the textureID and slot will always be the same
    glActiveTexture(GL_TEXTURE0 + textureID); 
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
      std::println("Failed to load texture");
    }

    glBindTexture(GL_TEXTURE_2D, 0);
  }

  Texture::~Texture() {
    glDeleteTextures(1, &textureID);
  }

  void Texture::Bind(unsigned int slot) const {
    m_Slot = slot;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
  }

  void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
  }

}