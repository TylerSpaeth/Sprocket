#ifndef APPLICATION_H
#define APPLICATION_H

#include "Macros.h"
#include "Window.h"

namespace Sprocket {

  class SPROCKET_API Application {
    private:
      Window* m_Window;

    public:
      Application(unsigned int windowXDimension, unsigned int windowYDimension, const char* windowTitle);
      virtual ~Application();
      void Run();
  };

  // Define in the code that is using Sprocket
  Application* CreateApplication();

}

#endif