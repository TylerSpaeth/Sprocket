#ifndef WINDOW_H
#define WINDOW_H

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"
#include "Events/Event.h"

#include "Macros.h"

#include <string>
#include <functional>

namespace Sprocket {

  // Window is a singleton
  class SPROCKET_API Window {
    private:
      unsigned int m_XDimension;
      unsigned int m_YDimension;
      GLFWwindow* m_Window;
      std::function<void(Event&)> m_EventCallback; // Function to call when an even occurs
    public:
      Window(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle);
      void OnClose();
      void OnUpdate();
      void RegisterEventCallback(const std::function<void(Event&)>eventCallback);
  };

}

#endif