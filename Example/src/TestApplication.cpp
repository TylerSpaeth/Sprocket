#include "Sprocket.h"
#include <functional>
#include <iostream>

class TestApplication : public Sprocket::Application {
  int id6 = 0;
  int id5 = 0;
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

      auto id3 = scene->CreateEntity();
      scene->AddComponent(id3, CameraComponent());

      scene->SetEntityParent(id2, id3);
      scene->SetEntityParent(id2, id);
      auto t = scene->GetTransform(id2);
      t.position.x += 100;
      scene->UpdateComponent(id2, t);
      
      // Checkered background
      {
        for(int i = -2000; i < 2000; i+=200) {
          for(int j = -2000; j < 2000; j+=200) {
            auto id4 = scene->CreateEntity();
            qcomp.texturePath = "";
            scene->AddComponent(id4,qcomp);
            TransformComponent tr = scene->GetTransform(id4);
            tr.position.x = i;
            tr.position.y = j;
            tr.position.z-=.01;
            scene->UpdateComponent(id4,tr);
          }
        }

        for(int i = -1900; i < 2100; i+=200) {
          for(int j = -1900; j < 2100; j+=200) {
            auto id4 = scene->CreateEntity();
            qcomp.texturePath = "";
            scene->AddComponent(id4,qcomp);
            TransformComponent tr = scene->GetTransform(id4);
            tr.position.x = i;
            tr.position.y = j;
            tr.position.z-=.01;
            scene->UpdateComponent(id4,tr);
          }
        }
      }

      id5 = scene->CreateEntity();
      qcomp.quadColor = {1,0,0,1};
      scene->AddComponent(id5, qcomp);
      BoxColliderComponent b;
      TransformComponent transf = scene->GetTransform(id5);
      transf.position.x = -200;
      scene->UpdateComponent(id5,transf);
      b.height = 100;
      b.width = 100;
      scene->AddComponent(id5, b);

      id6 = scene->CreateEntity();
      qcomp.quadColor = {0,1,0,1};
      BoxColliderComponent b2;
      b2.height = 100;
      b2.width = 100;
      scene->AddComponent(id6,qcomp);
      scene->AddComponent(id6,b2);
      TransformComponent tc = scene->GetTransform(id6);
      tc.scale.x = .5;
      tc.scale.y = .5;
      tc.rotation.z = 45;
      scene->UpdateComponent(id6,tc);
      
    }

    bool increase = true;
    void Update(float deltaTime) {
      using namespace Sprocket;
      //std::cout << (int) (1000000 / (deltaTime * 1000000)) << "\n";
      
      {
        Scene* scene = SceneManager::GetActiveScene();
        auto c1 = scene->GetBoxCollider(id5);
        auto t1 = scene->GetTransform(id5);
        auto c2 = scene->GetBoxCollider(id6);
        auto t2 = scene->GetTransform(id6);

        std::cout << Collision(c1,t1,c2,t2).Collides() << "\n";

        if(Input::IsKeyPressed(KEY_UP)) {
          Scene* scene = SceneManager::GetActiveScene();
          t2.position.y += deltaTime*80;
          scene->UpdateComponent(id6,t2);
        }
        if(Input::IsKeyPressed(KEY_DOWN)) {
          Scene* scene = SceneManager::GetActiveScene();
          t2.position.y -= deltaTime*80;
          scene->UpdateComponent(id6,t2);
        }
        if(Input::IsKeyPressed(KEY_LEFT)) {
          Scene* scene = SceneManager::GetActiveScene();
          t2.position.x -= deltaTime*80;
          scene->UpdateComponent(id6,t2);
        }
        if(Input::IsKeyPressed(KEY_RIGHT)) {
          Scene* scene = SceneManager::GetActiveScene();
          t2.position.x += deltaTime*80;
          scene->UpdateComponent(id6,t2);
        }
      }


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

      // Moving the camera around the scene
      if(Sprocket::Input::IsKeyPressed(KEY_W)) {
        Scene* scene = SceneManager::GetActiveScene();
        TransformComponent t = scene->GetTransform(2);
        t.position.y += 80*deltaTime;
        scene->UpdateComponent(2,t);
      } 
      if(Sprocket::Input::IsKeyPressed(KEY_A)) {
        Scene* scene = SceneManager::GetActiveScene();
        TransformComponent t = scene->GetTransform(2);
        t.position.x -= 80*deltaTime;
        scene->UpdateComponent(2,t);
      }
      if(Sprocket::Input::IsKeyPressed(KEY_S)) {
        Scene* scene = SceneManager::GetActiveScene();
        TransformComponent t = scene->GetTransform(2);
        t.position.y -= 80*deltaTime;
        scene->UpdateComponent(2,t);
      }
      if(Sprocket::Input::IsKeyPressed(KEY_D)) {
        Scene* scene = SceneManager::GetActiveScene();
        TransformComponent t = scene->GetTransform(2);
        t.position.x += 80*deltaTime;
        scene->UpdateComponent(2,t);
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