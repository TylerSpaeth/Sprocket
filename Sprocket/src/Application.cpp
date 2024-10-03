#include "Application.h"

namespace Sprocket {

  Application::Application(unsigned int windowXDimension, unsigned int windowYDimension, const char* windowTitle) {
    m_Window = new Window(windowXDimension, windowYDimension, windowTitle);
  }

  Application::~Application() {}

  void Application::Run() {
    while(true) {}
  }

  // Returns the time since this function was last called in terms of microseconds
  int64_t Application::GetTimeSinceLastChecked() {
    auto currentMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      auto elapsed = currentMicro - m_LastTimeChecked;
      m_LastTimeChecked = currentMicro;
      return elapsed;
  }

}