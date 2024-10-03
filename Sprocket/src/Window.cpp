#include "Window.h"

#include <vector>

namespace Sprocket {

  static GLFWwindow* InitGLFWwindow(const unsigned int xDimension, const unsigned int yDimension, const char* windowTitle) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(xDimension,yDimension,windowTitle,NULL,NULL);
    if(window == NULL) {
      glfwTerminate();
    }
    // Make the window we created the current context on the current thread
    glfwMakeContextCurrent(window);

    // Disables vsync
    glfwSwapInterval(0);

    // Locks aspect ratio to whatever was set intially
    glfwSetWindowAspectRatio(window, xDimension, yDimension);

    return window;
  }

  // Set the size of the OpenGL viewport when the window is resized
  static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width,height);
  }

  // This vector stores the keys that have been pressed and need to be cleaned between frames
  std::vector<int> clearKeys;
  std::vector<int> pressedKeys;

  // This updates the key states in the input object that is stored in the glfw user pointer
  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
    Input* input = (Input*)glfwGetWindowUserPointer(window);
    input->UpdateKeyState(key, action);
    if(action == GLFW_RELEASE) {
      clearKeys.push_back(key);
    }
    else if(action == GLFW_PRESS) {
      pressedKeys.push_back(key);
    }
  }

  // This clears all key for the keys that are stored in the keys vector
  void Window::ClearInputs() {
    Input* input = (Input*)glfwGetWindowUserPointer(m_Window);
    for(int i : clearKeys) {
      input->UpdateKeyState(i, -1);
    }
    clearKeys.clear();
    for(int i : pressedKeys) {
      input->UpdateKeyState(i, GLFW_REPEAT);
    }
    pressedKeys.clear();
  }

  static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    Input* input = (Input*)glfwGetWindowUserPointer(window);
    input->UpdateCursorPosition(xpos, ypos);
  }

  static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Input* input = (Input*)glfwGetWindowUserPointer(window);
    input->UpdateMouseButtonState(button, action);
  }

  static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Input* input = (Input*)glfwGetWindowUserPointer(window);
    input->UpdateMouseScrollOffset(xoffset, yoffset);
  }

  static void RegisterCallbacks(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);
  }

  Window::Window(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle) : m_XDimension(xDimension), m_YDimension(yDimension) {
    m_Window = InitGLFWwindow(xDimension, yDimension, windowTitle.c_str());
    m_Input = new Input(m_Window);
    glfwSetWindowUserPointer(m_Window, m_Input);
    RegisterCallbacks(m_Window);

    // Initialize GLAD
    // MUST BE DONE BEFORE ANY OPENGL CALLS INCLUDING INTIALIZING RENDERER
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  }

  void Window::SetShouldClose() {
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
  }

  bool Window::ShouldClose() {
    return glfwWindowShouldClose(m_Window);
  }

  void Window::Close() {
    glfwTerminate();
  }

  void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
  }

  void Window::PollEvents() {
    glfwPollEvents();
  }

  // Returns the time since this function was last called in terms of microseconds
  int64_t Window::GetTimeSinceLastChecked() {
    auto currentMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      auto elapsed = currentMicro - m_LastTimeChecked;
      m_LastTimeChecked = currentMicro;
      return elapsed;
  }

  void Window::InitializeRenderer(unsigned int maxQuads) {
    m_Renderer = new Renderer(maxQuads, m_XDimension, m_YDimension);
  }

}