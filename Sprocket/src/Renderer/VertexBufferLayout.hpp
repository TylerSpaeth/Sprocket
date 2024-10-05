#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>
#include <stdexcept>
#include "ThirdParty/glad/glad.h"

namespace Sprocket {

  struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
      switch(type) {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
      }
      return 0;
    }
  };

  class VertexBufferLayout {
    private:
      std::vector<VertexBufferElement> elements;
      unsigned int stride;
    public:
      VertexBufferLayout() : stride(0) {

      };

      template<typename T>
      void Push(unsigned int count) {
        std::runtime_error();
      }

      inline const std::vector<VertexBufferElement> GetElements() const {return elements;}
      inline unsigned int GetStride() const {return stride;}
  };

  template<>
  inline void VertexBufferLayout::Push<float>(unsigned int count) {
    elements.push_back({GL_FLOAT, count, GL_FALSE});
    stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
  }

  template<>
  inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
    elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
  }

  template<>
  inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
    elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
  }

}

#endif
