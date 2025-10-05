#ifndef BIRD_HPP
#define BIRD_HPP

#include "Sprocket.h"

namespace Sprocket {

    class Bird : public Entity {

    private:

        bool jumping = false;
        bool atApex = false;
        float timeAtApex = 0;
        float apexDuration = .05;
        float jumpSpeed = 4;
        float jumpHeight = 1;
        float heightJumped = 0;

    public:

        std::function<void()> gameOverCallback;

        Bird() {
            AddComponent<QuadRendererComponent>();
            AddComponent<CircleColliderComponent>();
            AddComponent<SoundComponent>();
            GetComponent<TransformComponent>()->LocalScale() = { 0.5,0.5,1 };
        }

        void Start() override {
            Sprite sprite;
            sprite.texturePath = "textures/Circle.png";
            GetComponent<QuadRendererComponent>()->SetSprite(sprite);
            GetComponent<SoundComponent>()->SetFilepath("sounds/pixel-jump-319167.mp3");
            GetComponent<SoundComponent>()->SetVolume(0.5f);
        }

        void Update(float deltaTime) {

            auto collider = GetComponent<CircleColliderComponent>();
            if (collider->CollidesWithAnything()) {
                gameOverCallback();
                return;
            }

            if (atApex) {
                timeAtApex += deltaTime;
                if (timeAtApex < apexDuration) {
                    return;
                }
                atApex = false;
                timeAtApex = 0;
            }

            if (!jumping && Input::IsKeyPressed(KEY_SPACE)) {
                jumping = true;
                GetComponent<SoundComponent>()->Play();
            }

            float move = 0;

            if (jumping) {
                move = std::min(jumpHeight - heightJumped, jumpSpeed * deltaTime);
                if (move == jumpSpeed * deltaTime) {
                    heightJumped += jumpSpeed * deltaTime;
                }
                else {
                    heightJumped = 0;
                    jumping = false;
                    atApex = true;
                }
            }
            else {
                move = -jumpSpeed * deltaTime;
            }


            GetComponent<TransformComponent>()->LocalPosition().y += move;
        }

    };

}

#endif