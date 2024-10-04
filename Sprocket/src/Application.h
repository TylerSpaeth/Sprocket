#ifndef APPLICATION_H
#define APPLICATION_H

#include "Macros.h"
#include "Events/Event.h"

#include <chrono>
#include <functional>
#include <vector>

namespace Sprocket {

  class SPROCKET_API Application {
    private:
      bool m_AppRunning;
      int64_t m_LastTimeChecked = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      int64_t GetTimeSinceLastChecked();
      std::vector<std::function<void(Event&)>> m_EventCallbacks;
    public:
      Application();
      virtual ~Application();
      void Run();
      void OnEvent(Event& event);
      void RegisterEventCallback(std::function<void(Event&)> eventCallback);
  };

  // Define in the code that is using Sprocket
  Application* CreateApplication();

}

#endif