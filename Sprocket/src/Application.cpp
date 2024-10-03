#include <functional>
#include <iostream>

#include "Application.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

namespace Sprocket {

  Application::Application(unsigned int windowXDimension, unsigned int windowYDimension, const char* windowTitle, unsigned int maxQuads) {
    m_Window = new Window(windowXDimension, windowYDimension, windowTitle);
    m_Window->RegisterEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    
    m_Renderer = new Renderer(maxQuads, windowXDimension, windowYDimension);
  }

  Application::~Application() {}

  void Application::Run() {
    // TODO everything here is just for testing
    while(true) {
    m_Renderer->Clear();
    m_Renderer->Draw();
    m_Window->OnUpdate();
    }
  }

  // Returns the time since this function was last called in terms of microseconds
  int64_t Application::GetTimeSinceLastChecked() {
    auto currentMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      auto elapsed = currentMicro - m_LastTimeChecked;
      m_LastTimeChecked = currentMicro;
      return elapsed;
  }

  void Application::OnEvent(Event& event) {
    // TODO make this Post to other handlers
    if(event.GetEventType() == WINDOW_CLOSE) {
      std::cout << "Closing\n";
    }
  }

}