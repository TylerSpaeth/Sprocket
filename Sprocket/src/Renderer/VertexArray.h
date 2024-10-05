#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.hpp"

namespace Sprocket {

  class VertexArray {
    private:
      unsigned int rendererID;
    public:
      VertexArray();
      ~VertexArray();
      void Bind() const;
      void Unbind() const;
      void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
  };

}

#endif