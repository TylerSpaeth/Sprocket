#include "VertexBufferLayout.h"

#include "ThirdParty/glad/glad.h"

namespace Sprocket {

  unsigned int VertexBufferLayout::GetSizeOfType(unsigned int type) {
    switch(type) {
      case GL_FLOAT: return 4;
      case GL_UNSIGNED_INT: return 4;
      case GL_UNSIGNED_BYTE: return 1;
    }
    return 0;
  }

  template<>
  inline void VertexBufferLayout::Push<float>(unsigned int count) {
    elements.push_back({GL_FLOAT, count, GL_FALSE});
    stride += GetSizeOfType(GL_FLOAT) * count;
  }

  template<>
  inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
    elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    stride += GetSizeOfType(GL_UNSIGNED_INT) * count;
  }

  template<>
  inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
    elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    stride += GetSizeOfType(GL_UNSIGNED_BYTE) * count;
  }

}