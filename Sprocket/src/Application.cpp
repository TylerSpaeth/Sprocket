#include "Application.h"

namespace Sprocket {

  Application::Application(unsigned int windowXDimension, unsigned int windowYDimension, const char* windowTitle) {
    m_Window = new Window(windowXDimension, windowYDimension, windowTitle);
  }

  Application::~Application() {}

  void Application::Run() {
    while(true) {}
  }

}