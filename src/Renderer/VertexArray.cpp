#include "VertexArray.h"

VertexArray::VertexArray() {
  glGenVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::Bind() const {
  glBindVertexArray(rendererID);
}

void VertexArray::Unbind() const {
  glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) {
  Bind();
  vertexBuffer.Bind();
  const std::vector<VertexBufferElement>& elements = layout.GetElements();
  unsigned int offset = 0;
  for(unsigned int i = 0; i < elements.size(); i++) {
    const VertexBufferElement element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(),
        (const void*)offset);
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
}