#include "Application.h"
#include "Events/EventHandler.h"
#include "Events/ApplicationEvent.h"

#include <iostream>

namespace Sprocket {

  Application::Application() : m_AppRunning(true) {
    std::cout << "Sprocket: Startup\n";
  }

  Application::~Application() {}
  void Application::Start() {}
  void Application::Update(float deltaTime) {}

  void Application::Run() {
    this->Start();

    // TODO everything here is just for testing
    while(m_AppRunning) {
      float deltaTime = GetTimeSinceLastChecked() / 1000000.0f;
      this->Update(deltaTime);
      ApplicationUpdateEvent event(deltaTime);
      OnEvent(event);
    }
  }

  void Application::OnEvent(Event& event) {
    
    EventHandler eventHandler(event);

    // Traverse the callbacks in reverse order. Right now this is done so we can register the window
    // and renderer first. That way we can assure they receive events, mainly update, last
    for(int i = m_EventCallbacks.size()-1; i >= 0; i--) {

      if(event.IsCategory(m_EventCallbacks[i].second)) {

         // Post the event to the subscriber
        eventHandler.Post(m_EventCallbacks[i].first);
      }
    }

    if(event.GetEventType() == EventType::WINDOW_CLOSE) {
      std::cout << "Sprocket: Shutdown\n";
      m_AppRunning = false;
    }
  }

  void Application::RegisterEventCallback(std::function<void(Event&)> eventCallback, EventCategory category) {
    m_EventCallbacks.push_back(std::pair(eventCallback, category));
  }

  int64_t Application::GetTimeSinceLastChecked() {
    auto currentMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      auto elapsed = currentMicro - m_LastTimeChecked;
      m_LastTimeChecked = currentMicro;
      return elapsed;
  }

}