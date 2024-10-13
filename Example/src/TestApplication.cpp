#include "Sprocket.h"
#include <functional>
#include <iostream>

class TestApplication : public Sprocket::Application {

  public:
    TestApplication() : Sprocket::Application() {}
    ~TestApplication() {}
    void Start() {
      using namespace Sprocket;
      /*Scene* scene = SceneManager::GetActiveScene();
      RootEntity* const root = scene->GetSceneRoot();
      Entity* e = new Entity(root);
      Entity* e2 = new Entity(e);
      e->GetLocalTransform().m_Scale.x = 2;
      e2->GetLocalTransform().m_Scale.x = 0.75;
      std::cout << e2->GetGlobalTransform().m_Scale.x << "\n";*/

      RenderNewEvent* ev = new RenderNewEvent(100,1.0f);
      this->OnEvent(*ev);

      RenderUpdateEvent* ev2 = new RenderUpdateEvent(RenderUpdateType::MODEL_MATRIX);
      ev2->m_QuadIndex = ev->m_QuadID;
      ev2->m_Matrix = glm::translate(glm::mat4(1.0f), glm::vec3(100, 200, 0));
      this->OnEvent(*ev2);
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
  app->RegisterEventCallback(Window::OnEvent);

  Input::Init();
  app->RegisterEventCallback(Input::OnEvent);
  
  Renderer::Init(100000, 1066, 600);
  app->RegisterEventCallback(Renderer::OnEvent);

  Renderer::AddTexture("../res/textures/BiggerBetterTree.png", 1);
  Renderer::UpdateTextureUniform(1);
  /*for(int i = 0; i < 100000; i++) {
    auto index = Renderer::AddQuad(100, 1);
    Renderer::SetQuadModelMatrix(index, glm::translate(glm::mat4(1.0f), glm::vec3(0, i, 0)));
  }*/

  
  
  SceneManager::Init();
  app->RegisterEventCallback(SceneManager::OnEvent);

  return app;
}