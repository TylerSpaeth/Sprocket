#include "Sprocket.h"
#include <functional>
#include <iostream>
#include <random>

class TestApplication : public Sprocket::Application {

  unsigned int camera;

  unsigned int user;
  unsigned int top;
  unsigned int right;
  unsigned int bottom;
  unsigned int left;

  unsigned int redBox;

  unsigned int tileMap;

  public:
    TestApplication() : Sprocket::Application() {}
    ~TestApplication() {}
    void Start() {
      using namespace Sprocket;
      //Scene* scene = SceneManager::GetActiveScene();
      Scene* scene = new Scene();

      {
        user = scene->CreateEntity();
        QuadRendererComponent qcomp(100);
        scene->AddComponent(user,qcomp);

        top = scene->CreateEntity();
        scene->SetEntityParent(top,user);
        TransformComponent t1;
        t1.position.y = 50.5;
        scene->UpdateComponent(top,t1);

        right = scene->CreateEntity();
        scene->SetEntityParent(right,user);
        TransformComponent t2;
        t2.position.x = 50.5;
         scene->UpdateComponent(right, t2);

        bottom = scene->CreateEntity();
        scene->SetEntityParent(bottom,user);
        TransformComponent t3;
        t3.position.y = -50.5;
         scene->UpdateComponent(bottom, t3);

        left = scene->CreateEntity();
        scene->SetEntityParent(left,user);
        TransformComponent t4;
        t4.position.x = -50.5;
         scene->UpdateComponent(left, t4);

        BoxColliderComponent h;
        h.height = .5;
        h.width = 100;
        scene->AddComponent(top, h);
        scene->AddComponent(bottom, h);
      
        BoxColliderComponent v;
        v.height = 100;
        v.width = .5;
        scene->AddComponent(left, v);
        scene->AddComponent(right, v);

      }

      {
        redBox = scene->CreateEntity();
        QuadRendererComponent qcomp(50);
        qcomp.quadColor = {1,0,0,1};
        scene->AddComponent(redBox,qcomp);
        BoxColliderComponent bcomp;
        bcomp.width = 50;
        bcomp.height = 50;
        scene->AddComponent(redBox,bcomp);
        TransformComponent tcomp = scene->GetComponent<TransformComponent>(redBox);
        
        std::random_device r;
        std::mt19937 gen(r());
        std::uniform_real_distribution<float> distx(-424,424);
        std::uniform_real_distribution<float> disty(-224,224);
        tcomp.position.x = distx(gen);
        tcomp.position.y = disty(gen);

        scene->UpdateComponent(redBox,tcomp);

      }

      // TileMap of trees
      {
        TileMapComponent tile;
        tile.spriteMapPath = "../res/tilemaps/SpriteTileMap.txt";
        tile.colliderMapPath = "../res/tilemaps/ColliderTileMap.txt";
        //tile.spriteMapPath = "../res/tilemaps/MazeSprite.txt";
        //tile.colliderMapPath = "../res/tilemaps/MazeCollider.txt";
        tile.tileSize = 100;
        
        SpriteComponent circle;
        circle.texturePath = "../res/textures/Circle.png";

        SpriteComponent grass;
        grass.texturePath = "../res/textures/NewDirtTopDownTiles.png";
        grass.xDimension = 960;
        grass.yDimension = 960;
        grass.xUVCoords = {479,479,383,383};
        grass.yUVCoords = {287,191,191,287};

        SpriteComponent dirt;
        dirt.texturePath = "../res/textures/NewDirtTopDownTiles.png";
        dirt.xDimension = 960;
        dirt.yDimension = 960;
        dirt.xUVCoords = {383,383,287,287};
        dirt.yUVCoords = {863,767,767,863};

        tile.sprites.at(0) = grass;
        tile.sprites.at(1) = dirt;
        
        tileMap = scene->CreateEntity();
        scene->AddComponent(tileMap,tile);
        auto transform = scene->GetComponent<TransformComponent>(tileMap);
        transform.position.z -= .01;
        scene->UpdateComponent(tileMap,transform);
      }

      camera = scene->CreateEntity();
      scene->AddComponent(camera, CameraComponent());

      SceneManager::AddScene(1,scene);
      SceneManager::SetActiveScene(1);

    }

    void Update(float deltaTime) {
      using namespace Sprocket;
      Scene* scene = SceneManager::GetActiveScene();

      // Print frame time and fps
      std::cout << deltaTime * 1000 << "ms " <<(int) (1000000 / (deltaTime * 1000000)) << "fps\n";

      // Close on escape
      if(Input::IsKeyPressed(KEY_ESCAPE)) {
        WindowCloseEvent* wc = new WindowCloseEvent();
        OnEvent(*wc);
      }

      // Have the camera stay centered on the user's cube
      scene->UpdateComponent(camera, scene->GetComponent<TransformComponent>(user));

      if(scene->CheckCollides(redBox)) {
        scene->DeleteEntity(redBox);

        redBox = scene->CreateEntity();
        QuadRendererComponent qcomp(50);
        qcomp.quadColor = {1,0,0,1};
        scene->AddComponent(redBox,qcomp);
        BoxColliderComponent bcomp;
        bcomp.width = 50;
        bcomp.height = 50;
        scene->AddComponent(redBox,bcomp);
        TransformComponent tcomp = scene->GetComponent<TransformComponent>(redBox);
        
        std::random_device r;
        std::mt19937 gen(r());
        std::uniform_real_distribution<float> distx(-450,450);
        std::uniform_real_distribution<float> disty(-224,224);
        tcomp.position.x = distx(gen);
        tcomp.position.y = disty(gen);

        scene->UpdateComponent(redBox,tcomp);

        QuadRendererComponent userQComp = scene->GetComponent<QuadRendererComponent>(user);
        if(userQComp.quadColor.g > 0 && userQComp.quadColor.b > 0) {
          userQComp.quadColor.g -= .05;
          userQComp.quadColor.b -= .05;
          scene->UpdateComponent(user,userQComp);
        }

      }

      {
        TransformComponent tcomp = scene->GetComponent<TransformComponent>(user);

        if(Input::IsKeyPressed(KEY_W) && !scene->CheckCollides(top)) {
          tcomp.position.y += deltaTime * 150;
          scene->UpdateComponent(user,tcomp);
        }
        if(Input::IsKeyPressed(KEY_S) && !scene->CheckCollides(bottom)) {
          tcomp.position.y -= deltaTime * 150;
          scene->UpdateComponent(user,tcomp);
        }
        if(Input::IsKeyPressed(KEY_A) && !scene->CheckCollides(left)) {
          tcomp.position.x -= deltaTime * 150;
          scene->UpdateComponent(user,tcomp);
        }
        if(Input::IsKeyPressed(KEY_D) && !scene->CheckCollides(right)) {
          tcomp.position.x += deltaTime * 150;
          scene->UpdateComponent(user,tcomp);
        }
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

  SceneManager::Init(std::bind(&Application::OnEvent, app, std::placeholders::_1));
  app->RegisterEventCallback(SceneManager::OnEvent, EventCategory::UNCATEGORIZED);

  return app;
}