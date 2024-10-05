#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>
#include <stdexcept>

namespace Sprocket {

  struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
  };

  class VertexBufferLayout {
    private:
      std::vector<VertexBufferElement> elements;
      unsigned int stride;
    public:
      VertexBufferLayout() : stride(0) {}

      static unsigned int GetSizeOfType(unsigned int type);

      template<typename T>
      void Push(unsigned int count) {
        std::runtime_error();
      }

      inline const std::vector<VertexBufferElement> GetElements() const {return elements;}
      inline unsigned int GetStride() const {return stride;}
  };


}

#endif
