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
      e->GetLocalTransform().position = glm::vec3(50,50,0);
      e->GetLocalTransform().scale.x = 1.5;
      e->GetLocalTransform().rotation.z = 45;

      QuadRendererComponent qcomp;
      qcomp.size = 100;
      //qcomp.texturePath = "../res/textures/BiggerBetterTree.png";
      qcomp.quadColor = {0.5, 0.5, 0, 1};
      qcomp.modified = true;

      /*for(int i = 0; i < 100; i++) {
        Entity* ent = new Entity(e);
        ent->AddComponent(qcomp);
        ent->GetLocalTransform().position.y += i;
        ent->GetLocalTransform().rotation.z += i;
        ent->GetLocalTransform().modified = true;
      }*/

      e->AddComponent(qcomp);

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
        RootEntity* const root = scene->GetSceneRoot();
        Entity* e = root->GetChildren().at(0);
        e->GetLocalTransform().rotation.z += deltaTime*40;
        e->GetLocalTransform().modified = true;
      }

      // Change the color of the first quad if x is pressed
      if(Sprocket::Input::IsKeyPressed(Sprocket::KEY_X)) {
        Scene* scene = SceneManager::GetActiveScene();
        RootEntity* const root = scene->GetSceneRoot();
        Entity* e = root->GetChildren().at(0);
        QuadRendererComponent* q = (QuadRendererComponent*)e->GetComponents().at(0);
        if(increase) {
          q->quadColor.x = q->quadColor.x + deltaTime;
        }
        else {
          q->quadColor.x = q->quadColor.x - deltaTime;
        }
        if(q->quadColor.x >= .99) {
          increase = false;
        }
        else if(q->quadColor.x <= .01) {
          increase = true;
        }
        q->modified = true;
        
      }

      if(Sprocket::Input::IsKeyPressed(KEY_D)) {
        RenderDeleteEvent* e = new RenderDeleteEvent(0);
        Renderer::OnEvent(*e);

        RenderNewEvent* rn = new RenderNewEvent(100);
        Renderer::OnEvent(*rn);

        //std::cout << rn->m_QuadID << "\n";
        
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