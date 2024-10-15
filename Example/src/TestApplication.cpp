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
      e->GetLocalTransform().position = glm::vec3(50,100,0);
      e->GetLocalTransform().scale.x = 1.5;
      e->GetLocalTransform().rotation.z = 45;

      QuadRendererComponent qcomp;
      qcomp.size = 100;
      qcomp.texturePath = "../res/textures/BiggerBetterTree.png";
      qcomp.quadColor = {0.5, 0.5, 0, 1};
      qcomp.modified = true;

      /*for(int i = 0; i < 100000; i++) {
        Entity* ent = new Entity(root);
        ent->AddComponent(qcomp);
        ent->GetLocalTransform().position.y += i;
        ent->GetLocalTransform().modified = true;
      }*/

      e->AddComponent(qcomp);

    }
    void Update(float deltaTime) {
      using namespace Sprocket;
      std::cout << (int) (1000000 / (deltaTime * 1000000)) << "\n";
      if(Sprocket::Input::IsKeyPressed(Sprocket::KEY_SPACE)) {
        Scene* scene = SceneManager::GetActiveScene();
        RootEntity* const root = scene->GetSceneRoot();
        Entity* e = root->GetChildren().at(0);
        e->GetLocalTransform().rotation.z += deltaTime*40;
        e->GetLocalTransform().modified = true;
      }
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
  
  Renderer::Init(500000, 1066, 600);
  app->RegisterEventCallback(Renderer::OnEvent, EventCategory::UNCATEGORIZED);

  SceneManager::Init();
  SceneManager::RegisterEventCallback(std::bind(&Application::OnEvent, app, std::placeholders::_1));
  app->RegisterEventCallback(SceneManager::OnEvent, EventCategory::UNCATEGORIZED);

  return app;
}