#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

namespace Sprocket {

  class VertexBuffer {
    private:
      unsigned int rendererID;
    public:
      VertexBuffer(const void* data, unsigned int size);
      ~VertexBuffer();
      void Bind() const;
      void Unbind() const;
  };

}

#endif