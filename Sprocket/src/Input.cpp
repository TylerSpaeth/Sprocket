#include "Input.h"

namespace Sprocket {

  Input::Input(GLFWwindow* window) : m_Window(window){
    for(unsigned int i : s_Keycodes) {
      m_Keys.insert(std::pair<unsigned int, KeyState>(i, {false,false,false}));
    }
    for(unsigned int i : s_MouseButtonCodes) {
      m_MouseButtons.insert(std::pair<unsigned int, MouseButtonState>(i, {false,false}));
    }
  }

  void Input::UpdateKeyState(int key, int action) {
    switch(action) {
          // Sets just pressed to true
          case GLFW_PRESS:
            m_Keys.at(key).Pressed = true;
            m_Keys.at(key).Released = false;
            m_Keys.at(key).Repeat = false;
            break;
          // Sets just repeat to true
          case GLFW_REPEAT:
            m_Keys.at(key).Pressed = false;
            m_Keys.at(key).Released = false;
            m_Keys.at(key).Repeat = true;
            break;
          // Sets just release to true
          case GLFW_RELEASE:
            m_Keys.at(key).Pressed = false;
            m_Keys.at(key).Released = true;
            m_Keys.at(key).Repeat = false;
            break;
          // Sets all flags to false
          case -1:
            m_Keys.at(key).Pressed = false;
            m_Keys.at(key).Released = false;
            m_Keys.at(key).Repeat = false;
            break;
        }
  }

  void Input::UpdateCursorPosition(double cursorXPos, double cursorYPos) {
    m_CursorXPos = cursorXPos;
    m_CursorYPos = cursorYPos;
  }

  void Input::EnableCursor() {
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  void Input::HideCursor() {
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  }

  void Input::DisableCursor() {
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  // Raw mouse input requires the cursor to be disabled to work
  void Input::EnableRawMouseInput() {
    if(glfwRawMouseMotionSupported())
      glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }

  void Input::DisableRawMouseInput() {
    if(glfwRawMouseMotionSupported())
      glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
  }

  void Input::UpdateMouseButtonState(int button, int action) {
    switch(action) {
      case GLFW_PRESS:
        m_MouseButtons.at(button).Pressed = true;
        m_MouseButtons.at(button).Released = false;
        break;
      case GLFW_RELEASE:
        m_MouseButtons.at(button).Pressed = false;
        m_MouseButtons.at(button).Released = true;
        break;
    }
  }

  void Input::UpdateMouseScrollOffset(double xOffset, double yOffset) {
    m_MouseScrollXOffset = xOffset;
    m_MouseScrollYOffset = yOffset;
  }

}