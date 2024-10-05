#include "Window.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvents.h"

#include <iostream>
#include <vector>


namespace Sprocket {

  ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// NONCLASS FUNCTIONS ////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

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
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent e((MouseButton) button);
        eventCallback(e);
        break;
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
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////// STATIC FUNCTIONS /////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  Window* Window::s_Instance = nullptr;
  void Window::Init(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle) {
    if(!s_Instance) {
      s_Instance = new Window();
      s_Instance->m_Window = InitGLFWwindow(xDimension, yDimension, windowTitle.c_str());
      glfwSetWindowUserPointer(s_Instance->m_Window, (void*)&s_Instance->m_EventCallback);
      RegisterCallbacks(s_Instance->m_Window);

      // Initialize GLAD
      // MUST BE DONE BEFORE ANY OPENGL CALLS INCLUDING INTIALIZING RENDERER
      gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// INSTANCE FUNCTIONS ////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  void Window::OnEventInstance(Event& event) {
    EventType type = event.GetEventType();
    switch(type) {
      case APP_UPDATE:
        OnUpdateInstance();
        break;
      case WINDOW_CLOSE:
        OnCloseInstance();
        break;
    }
  }

  void Window::OnCloseInstance() {
    glfwTerminate();
  }

  void Window::OnUpdateInstance() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
  }

  void Window::RegisterEventCallbackInstance(const std::function<void(Event&)> eventCallback) {
    m_EventCallback = eventCallback;
    KeyPressedEvent e(KEY_0);
    eventCallback(e);
  }

  void Window::EnableVSyncInstance(bool enable) {
    if(enable) {
      glfwSwapInterval(1);
    }
    else {
      glfwSwapInterval(0);
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

}