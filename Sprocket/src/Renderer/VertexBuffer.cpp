#include "VertexBuffer.h"
#include "ThirdParty/glad/glad.h"

namespace Sprocket {

  VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    glGenBuffers(1, &rendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    Unbind();
  }

  VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &rendererID);
  }

  void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
  }

  void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

}