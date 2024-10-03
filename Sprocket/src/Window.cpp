#include "Window.h"

#include <vector>
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvents.h"

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

  // This updates the key states in the input object that is stored in the glfw user pointer
  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod) {
    const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
    switch(action) {
      case GLFW_REPEAT:
      case GLFW_PRESS: {
        KeyPressedEvent e((Keycode) key);
        eventCallback(e);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent e((Keycode) key);
        eventCallback(e);
        break;
      }
    }
  }

  static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
    MouseMovedEvent e(xpos, ypos);
    eventCallback(e);
  }

  static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
    switch(action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent e((MouseButton) button);
        eventCallback(e);
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent e((MouseButton) button);
        eventCallback(e);
      }
    }
  }

  static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
    MouseScrolledEvent e(xoffset, yoffset);
    eventCallback(e);
  }

  static void WindowCloseCallback(GLFWwindow* window) {
    const std::function<void(Event&)> eventCallback = *(const std::function<void(Event&)>*) glfwGetWindowUserPointer(window);
    WindowCloseEvent e;
    eventCallback(e);
  }

  static void RegisterCallbacks(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetWindowCloseCallback(window, WindowCloseCallback);
  }

  Window::Window(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle) : m_XDimension(xDimension), m_YDimension(yDimension) {
    m_Window = InitGLFWwindow(xDimension, yDimension, windowTitle.c_str());
    glfwSetWindowUserPointer(m_Window, (void*)&m_EventCallback);
    RegisterCallbacks(m_Window);

    // Initialize GLAD
    // MUST BE DONE BEFORE ANY OPENGL CALLS INCLUDING INTIALIZING RENDERER
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  }

  void Window::OnClose() {
    glfwTerminate();
  }

  void Window::OnUpdate() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
  }

  void Window::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
    m_EventCallback = eventCallback;
    KeyPressedEvent e(KEY_0);
    eventCallback(e);
  }

}