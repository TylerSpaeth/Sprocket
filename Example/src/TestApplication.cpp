#include "Sprocket.h"
#include <functional>
#include <iostream>

class TestApplication : public Sprocket::Application {

  public:
    TestApplication() : Sprocket::Application() {}
    ~TestApplication() {}
    void Start() {

    }
    void Update(float deltaTime) {
      std::cout << (int) (1000000 / (deltaTime * 1000000)) << "\n";
    }
};

Sprocket::Application* Sprocket::CreateApplication() {

  TestApplication* app = new TestApplication();

  Window::Init(1066, 600, "Test Window");
  Window::EnableVSync(false);
  Window::RegisterEventCallback(std::bind(&Application::OnEvent, app, std::placeholders::_1));
  app->RegisterEventCallback(Window::OnEvent);

  Input::Init();
  app->RegisterEventCallback(Input::OnEvent);
  
  Renderer::Init(100000, 1066, 600);
  app->RegisterEventCallback(Renderer::OnEvent);

  Renderer::AddTexture("../res/textures/BiggerBetterTree.png", 1);
  Renderer::UpdateTextureUniform(1);
  Renderer::AddQuad(100, 1);

  SceneManager::Init();
  app->RegisterEventCallback(SceneManager::OnEvent);

  return app;
}