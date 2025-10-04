#ifndef PLAYER_H
#define PLAYER_H

#include "Sprocket.h"

namespace Sprocket {

    class Player : public Entity {

    private:
        float moveSpeed = 2.0f;
        float rotateSpeed = 60.0f;

    public:

        Player() {
            AddComponent<QuadRendererComponent>();
            AddComponent<BoxColliderComponent>();
            AddComponent<SoundComponent>();
        }

        void Update(float deltaTime) override {

            auto soundComponent = GetComponent<SoundComponent>();
            
            if (soundComponent->GetFilepath().empty()) {
               soundComponent->SetFilepath("sounds/chime.wav");
                soundComponent->SetVolume(0.5);
                soundComponent->SetLooping(true);
            }

            auto collides = GetComponent<BoxColliderComponent>()->CollidesWithAnything();
            auto qr = GetComponent<QuadRendererComponent>();
            if (qr) {
                if (qr->GetQuadColor() != glm::vec4(1, 0, 0, 1) && collides) {
                    qr->SetQuadColor({ 1,0,0,1 });

                    if (!soundComponent->IsPlaying()) {
                        soundComponent->Play();
                    }
                }
                else if (!collides && qr->GetQuadColor() == glm::vec4(1, 0, 0, 1)) {
                    qr->SetQuadColor({ 1,1,1,1 });
                    soundComponent->Stop();
                    soundComponent->Reset();
                }
            }

            if (Input::IsKeyPressed(KEY_W)) {
                GetComponent<TransformComponent>()->LocalPosition().y += moveSpeed * deltaTime;
            }
            if (Input::IsKeyPressed(KEY_S)) {
                GetComponent<TransformComponent>()->LocalPosition().y -= moveSpeed * deltaTime;
            }
            if (Input::IsKeyPressed(KEY_D)) {
                GetComponent<TransformComponent>()->LocalPosition().x += moveSpeed * deltaTime;
            }
            if (Input::IsKeyPressed(KEY_A)) {
                GetComponent<TransformComponent>()->LocalPosition().x -= moveSpeed * deltaTime;
            }
            if (Input::IsKeyPressed(KEY_Q)) {
                GetComponent<TransformComponent>()->LocalRotation().z -= rotateSpeed * deltaTime;
            }
            if (Input::IsKeyPressed(KEY_E)) {
                GetComponent<TransformComponent>()->LocalRotation().z += rotateSpeed * deltaTime;
            }

            if (Input::IsKeyPressed(KEY_0)) {
                RemoveComponent<QuadRendererComponent>();
            }
            if (Input::IsKeyPressed(KEY_9)) {
                AddComponent<QuadRendererComponent>();
            }

            if (Input::IsKeyPressed(KEY_HOME)) {
                GetComponent<TransformComponent>()->LocalScale() += .01;
            }
            if (Input::IsKeyPressed(KEY_END) && GetComponent<TransformComponent>()->LocalScale().x > .02) {
                GetComponent<TransformComponent>()->LocalScale() -= .01;
            }
            if (Input::IsKeyPressed(KEY_8)) {
                GetComponent<QuadRendererComponent>()->SetQuadColor({ 1,0,1,1 });
            }
            if (Input::IsKeyPressed(KEY_7)) {
                Sprite sprite;
                sprite.texturePath = "textures/BiggerBetterTree.png";
                GetComponent<QuadRendererComponent>()->SetSprite(sprite);
            }
        }

    };

}

#endif