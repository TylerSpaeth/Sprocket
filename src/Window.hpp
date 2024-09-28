#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "ThirdParty/GLFW/glfw3.h"

// Set the size of the OpenGL viewport when the window is resized
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width,height);
}

GLFWwindow* InitGLFWwindow(const unsigned int xDimension, const unsigned int yDimension) {
  glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(xDimension,yDimension,"LearnOpenGL",NULL,NULL);
	if(window == NULL) {
		glfwTerminate();
	}
  // Make the window we created the current context on the current thread
	glfwMakeContextCurrent(window);

  // Disables vsync
  glfwSwapInterval(0);

  // Locks aspect ratio to whatever was set intially
  glfwSetWindowAspectRatio(window, xDimension, yDimension);

  // Register the viewport callback function to make the window resizable
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
  
  return window;
}

#endif
