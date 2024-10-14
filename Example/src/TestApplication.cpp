#include "Sprocket.h"
#include <functional>
#include <iostream>

class TestApplication : public Sprocket::Application {

  public:
    TestApplication() : Sprocket::Application() {}
    ~TestApplication() {}
    void Start() {
      using namespace Sprocket;
      Scene* scene = SceneManager::GetActiveScene();
      RootEntity* const root = scene->GetSceneRoot();
      Entity* e = new Entity(root);
      e->GetLocalTransform().m_Position = glm::vec3(50,100,0);
      e->GetLocalTransform().m_Scale.x = .5;
      e->GetLocalTransform().m_Rotation.z = 45;

      QuadRenderer q(100);
      auto id = e->AddComponent(q);
      ((QuadRenderer&)e->GetComponent(id)).SetTexture("../res/textures/BiggerBetterTree.png");
    
    }
    void Update(float deltaTime) {
      //std::cout << (int) (1000000 / (deltaTime * 1000000)) << "\n";
    }
};

Sprocket::Application* Sprocket::CreateApplication() {

  TestApplication* app = new TestApplication();

  Window::Init(1066, 600, "Test Window");
  Window::EnableVSync(false);
  Window::RegisterEventCallback(std::bind(&Application::OnEvent, app, std::placeholders::_1));
  app->RegisterEventCallback(Window::OnEvent, EventCategory::UNCATEGORIZED);

  Input::Init();
  app->RegisterEventCallback(Input::OnEvent, EventCategory::APPLICATION);
  
  Renderer::Init(100000, 1066, 600);
  app->RegisterEventCallback(Renderer::OnEvent, EventCategory::UNCATEGORIZED);

  SceneManager::Init();
  SceneManager::RegisterEventCallback(std::bind(&Application::OnEvent, app, std::placeholders::_1));
  app->RegisterEventCallback(SceneManager::OnEvent, EventCategory::UNCATEGORIZED);

  return app;
}