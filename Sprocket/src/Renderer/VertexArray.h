#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"

namespace Sprocket {

  class VertexArray {
    private:
      unsigned int rendererID;
    public:
      VertexArray();
      ~VertexArray();
      void Bind() const;
      void Unbind() const;
  };

}

#endif