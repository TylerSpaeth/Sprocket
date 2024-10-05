#ifndef MOUSEBUTTONS_H
#define MOUSEBUTTONS_H

#include "ThirdParty/GLFW/glfw3.h"

namespace Sprocket {

  enum MouseButton {
    MOUSE_BTN_1 = GLFW_MOUSE_BUTTON_1,
    MOUSE_BTN_2 = GLFW_MOUSE_BUTTON_2,
    MOUSE_BTN_3 = GLFW_MOUSE_BUTTON_3,
    MOUSE_BTN_4 = GLFW_MOUSE_BUTTON_4,
    MOUSE_BTN_5 = GLFW_MOUSE_BUTTON_5,
    MOUSE_BTN_6 = GLFW_MOUSE_BUTTON_6,
    MOUSE_BTN_7 = GLFW_MOUSE_BUTTON_7,
    MOUSE_BTN_8 = GLFW_MOUSE_BUTTON_8,
    MOUSE_BTN_LAST = GLFW_MOUSE_BUTTON_LAST,
    MOUSE_BTN_LEFT = GLFW_MOUSE_BUTTON_LEFT,
    MOUSE_BTN_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
    MOUSE_BTN_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
  };

}

#endif