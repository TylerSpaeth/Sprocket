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
      inline static unsigned int s_XDimension = 0;
      inline static unsigned int s_YDimension = 0;
      inline static std::string s_WindowTitle = "Window";
      inline static Window* s_Instance = nullptr;
      GLFWwindow* m_Window;
      Input* m_Input;
      Renderer* m_Renderer;
      Window(const unsigned int xDimension, const unsigned int yDimension);
      ~Window();
      int64_t m_LastTimeChecked = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    public:
      Window(const Window&) = delete; // Prevent copying
      Window& operator=(const Window&) = delete; // Prevent assignment
      static void SetWindowAttributes(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle);
      static Window& GetInstance();
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