#include "VertexArray.h"

#include "ThirdParty/glad/glad.h"

namespace Sprocket {

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
  
}