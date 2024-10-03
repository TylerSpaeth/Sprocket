#ifndef WINDOW_H
#define WINDOW_H

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"
#include "Input.h"
#include "Renderer/Renderer.h"

#include "Macros.h"

#include <string>
#include <chrono>

namespace Sprocket {

  // Window is a singleton
  class SPROCKET_API Window {
    private:
      unsigned int m_XDimension;
      unsigned int m_YDimension;
      GLFWwindow* m_Window;
      Input* m_Input;
      Renderer* m_Renderer;
      int64_t m_LastTimeChecked = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    public:
      Window(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle);
      void SetShouldClose();
      bool ShouldClose();
      void Close();
      void SwapBuffers();
      void PollEvents();
      Input* GetInput() const {return m_Input;}
      Renderer* GetRenderer() const {return m_Renderer;}
      void ClearInputs();
      int64_t GetTimeSinceLastChecked();
      void InitializeRenderer(unsigned int maxQuads);
  };

}

#endif