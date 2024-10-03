#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>

#include "Macros.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Events/Event.h"

namespace Sprocket {

  class SPROCKET_API Application {
    private:
      Window* m_Window;
      Renderer* m_Renderer;
      int64_t m_LastTimeChecked = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    public:
      Application(unsigned int windowXDimension, unsigned int windowYDimension, const char* windowTitle, unsigned int maxQuads);
      virtual ~Application();
      void Run();
      int64_t GetTimeSinceLastChecked();
      void OnEvent(Event& event);
  };

  // Define in the code that is using Sprocket
  Application* CreateApplication();

}

#endif