#include "IndexBuffer.h"
#include "../ThirdParty/glad/glad.h"

namespace Sprocket {

  IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : count(count) {
    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &rendererID);
  }

  void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
  }

  void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

}