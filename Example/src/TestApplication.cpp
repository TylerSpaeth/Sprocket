#include "Sprocket.h"

class TestApplication : public Sprocket::Application {

  public:
    TestApplication(unsigned int windowXDimension, unsigned int windowYDimension, const char* windowTitle) : Sprocket::Application(windowXDimension, windowYDimension, windowTitle) {}
    ~TestApplication() {}

};

Sprocket::Application* Sprocket::CreateApplication() {
  return new TestApplication(1066, 600, "Test Window");
}