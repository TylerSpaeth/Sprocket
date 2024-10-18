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

      QuadRendererComponent qcomp;
      qcomp.size = 100;
      qcomp.texturePath = "../res/textures/BiggerBetterTree.png";
      qcomp.quadColor = {0.5, 0.5, 0, 1};

      auto id = scene->CreateEntity();
      scene->AddComponent(id, qcomp);

      auto id2 = scene->CreateEntity();
      scene->AddComponent(id2, qcomp);

      scene->SetEntityParent(id2, id);
      auto t = scene->GetTransform(id2);
      t.position.x += 100;
      scene->UpdateComponent(id2, t);
      
      
    }

    bool increase = true;
    void Update(float deltaTime) {
      using namespace Sprocket;
      //std::cout << (int) (1000000 / (deltaTime * 1000000)) << "\n";

      if(Input::IsKeyPressed(KEY_ESCAPE)) {
        WindowCloseEvent* wc = new WindowCloseEvent();
        OnEvent(*wc);
      }
      
      // Rotate the first child clockwise if space is pressed
      if(Sprocket::Input::IsKeyPressed(Sprocket::KEY_SPACE)) {
        Scene* scene = SceneManager::GetActiveScene();
        TransformComponent t = scene->GetTransform(0);
        t.rotation.z += deltaTime*40;
        scene->UpdateComponent(0,t);
      }

      // Change the color of the first quad if x is pressed
      if(Sprocket::Input::IsKeyPressed(Sprocket::KEY_X)) {
        Scene* scene = SceneManager::GetActiveScene();
        QuadRendererComponent q = scene->GetQuadRenderer(0);
        if(increase) {
          q.quadColor.x = q.quadColor.x + deltaTime;
        }
        else {
          q.quadColor.x = q.quadColor.x - deltaTime;
        }
        if(q.quadColor.x >= .99) {
          increase = false;
        }
        else if(q.quadColor.x <= .01) {
          increase = true;
        }
        scene->UpdateComponent(0,q);
        
      }

      if(Sprocket::Input::IsKeyPressed(KEY_D)) {
        RenderDeleteEvent* e = new RenderDeleteEvent(0);
        Renderer::OnEvent(*e);

        RenderNewEvent* rn = new RenderNewEvent(100);
        Renderer::OnEvent(*rn);
        
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