#ifndef WINDOW_H
#define WINDOW_H

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"
#include "Input.h"

#include <string>

// Window is a singleton
class Window {
  private:
    inline static unsigned int s_XDimension = 0;
    inline static unsigned int s_YDimension = 0;
    inline static std::string s_WindowTitle = "Window";
    inline static Window* s_Instance = nullptr;
    GLFWwindow* m_Window;
    Input* m_Input;
    Window(const unsigned int xDimension, const unsigned int yDimension);
    ~Window();
  public:
    Window(const Window&) = delete; // Prevent copying
    Window& operator=(const Window&) = delete; // Prevent assignment
    static void SetWindowAttributes(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle);
    static Window& GetInstance();
    GLFWwindow* GetWindow() const {return m_Window;}
    Input* GetInput() const {return m_Input;}
    static void ClearInputs(GLFWwindow* window);
};

#endif