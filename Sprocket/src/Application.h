#ifndef APPLICATION_H
#define APPLICATION_H

#include "Macros.h"
#include "Window.h"
#include "Renderer/Renderer.h"

namespace Sprocket {

  class SPROCKET_API Application {
    private:
      Window* m_Window;
      Renderer* m_Renderer;
      int64_t m_LastTimeChecked = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    public:
      Application(unsigned int windowXDimension, unsigned int windowYDimension, const char* windowTitle);
      virtual ~Application();
      void Run();
      int64_t GetTimeSinceLastChecked();
  };

  // Define in the code that is using Sprocket
  Application* CreateApplication();

}

#endif