#include "Sprocket.h"

class TestApplication : public Sprocket::Application {

  public:
    TestApplication(unsigned int windowXDimension, unsigned int windowYDimension, const char* windowTitle, unsigned int maxQuads) : Sprocket::Application(windowXDimension, windowYDimension, windowTitle, maxQuads) {}
    ~TestApplication() {}

};

Sprocket::Application* Sprocket::CreateApplication() {
  return new TestApplication(1066, 600, "Test Window", 100000);
}