#include "Application.h"
#include "Events/EventHandler.h"
#include "Events/ApplicationEvent.h"

#include <iostream>

namespace Sprocket {

  Application::Application() : m_AppRunning(true) {
    std::cout << "Sprocket: Startup\n";
  }

  Application::~Application() {}

  void Application::Run() {

    // TODO everything here is just for testing
    while(m_AppRunning) {
      float deltaTime = GetTimeSinceLastChecked() / 1000000.0f;
      ApplicationUpdateEvent event(deltaTime);
      OnEvent(event);
    }
  }

  void Application::OnEvent(Event& event) {
    // TODO make this Post to other handlers
    
    EventHandler eventHandler(event);

    // Traverse the callbacks in reverse order. This is done so that window the last things 
    // that happen are the renderer drawing and the buffers being swapped
    int callbackCount = m_EventCallbacks.size();
    for(int i = callbackCount-1; i >= 0; i--) {
      // Post the event to the subscriber
      eventHandler.Post(m_EventCallbacks[i]);
    }

    if(event.GetEventType() == WINDOW_CLOSE) {
      std::cout << "Sprocket: Closing\n";
      m_AppRunning = false;
    }
  }

  void Application::RegisterEventCallback(std::function<void(Event&)> eventCallback) {
    m_EventCallbacks.push_back(eventCallback);
  }

  // Returns the time since this function was last called in terms of microseconds
  int64_t Application::GetTimeSinceLastChecked() {
    auto currentMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      auto elapsed = currentMicro - m_LastTimeChecked;
      m_LastTimeChecked = currentMicro;
      return elapsed;
  }

}