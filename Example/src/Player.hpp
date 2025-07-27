#ifndef PLAYER_H
#define PLAYER_H

#include "Sprocket.h"

namespace Sprocket {
  
  class Player : public Entity {

    public:

      Player() {
        AddComponent<QuadRendererComponent>();
        AddComponent<BoxColliderComponent>();
      }

      void Update(float deltaTime) override {
        auto collides = GetComponent<BoxColliderComponent>()->CollidesWithAnything();
        auto qr = GetComponent<QuadRendererComponent>();
        if(qr->GetQuadColor() != glm::vec4(1,0,0,1) && collides) {
          qr->UpdateQuadColor({1,0,0,1});
        }
        else if(!collides && qr->GetQuadColor() == glm::vec4(1,0,0,1)) {
          qr->UpdateQuadColor({1,1,1,1});
        }

        if(Input::IsKeyPressed(KEY_W)) {
          GetComponent<TransformComponent>()->LocalPosition().y += 2 * deltaTime;
        }
        if(Input::IsKeyPressed(KEY_S)) {
          GetComponent<TransformComponent>()->LocalPosition().y -= 2 * deltaTime;
        }
        if(Input::IsKeyPressed(KEY_D)) {
          GetComponent<TransformComponent>()->LocalPosition().x += 2 * deltaTime;
        }
        if(Input::IsKeyPressed(KEY_A)) {
          GetComponent<TransformComponent>()->LocalPosition().x -= 2 * deltaTime;
        }
        if(Input::IsKeyPressed(KEY_Q)) {
          GetComponent<TransformComponent>()->LocalRotation().z -= 60 * deltaTime;
        }
        if(Input::IsKeyPressed(KEY_E)) {
          GetComponent<TransformComponent>()->LocalRotation().z += 60 * deltaTime;
        }

        if(Input::IsKeyPressed(KEY_0)) {
          RemoveComponent<QuadRendererComponent>();
        }
        if(Input::IsKeyPressed(KEY_9)) {
          AddComponent<QuadRendererComponent>();
        }

        if(Input::IsKeyPressed(KEY_HOME)) {
          GetComponent<TransformComponent>()->LocalScale()+=.01;
        }
        if(Input::IsKeyPressed(KEY_END) && GetComponent<TransformComponent>()->LocalScale().x > .02) {
          GetComponent<TransformComponent>()->LocalScale()-=.01;
        }
        if(Input::IsKeyPressed(KEY_8)) {
          GetComponent<QuadRendererComponent>()->UpdateQuadColor({1,0,1,1});
        }
        if(Input::IsKeyPressed(KEY_7)) {
          GetComponent<QuadRendererComponent>()->UpdateTexturePath("../res/textures/BiggerBetterTree.png");
        }
      }

  };

}

#endif