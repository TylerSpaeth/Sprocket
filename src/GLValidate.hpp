#ifndef GLVALIDATE_H
#define GLVALIDATE_H

#include <iostream>

#include "ThirdParty/glad/glad.h"

static void GLClearError() {
  while(glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {
  while(GLenum error = glGetError() != GL_NO_ERROR) {
    std::cout << "OpenGL Error || " << error <<  "||\n";
  }
}

#endif