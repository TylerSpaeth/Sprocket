#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>
#include <functional>
#include <vector>

#include "Macros.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Events/Event.h"


namespace Sprocket {

  class SPROCKET_API Application {
    private:
      bool m_AppRunning;
      //Renderer* m_Renderer;
      int64_t m_LastTimeChecked = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      int64_t GetTimeSinceLastChecked();
      std::vector<std::function<void(Event&)>> m_EventCallbacks;
    public:
      Application(unsigned int windowXDimension, unsigned int windowYDimension, const char* windowTitle, unsigned int maxQuads);
      virtual ~Application();
      void Run();
      void OnEvent(Event& event);
      void RegisterEventCallback(std::function<void(Event&)> eventCallback);
  };

  // Define in the code that is using Sprocket
  Application* CreateApplication();

}

#endif