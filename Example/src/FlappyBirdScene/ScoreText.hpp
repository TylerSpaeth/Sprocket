#ifndef SCORETEXT_HPP
#define SCORETEXT_HPP

#include "Sprocket.h"

namespace Sprocket {

    class ScoreText : public Entity {

    private:

        int score;
        float elapsedTime;
        float initialDelay = 4.75;
        float delayBetweenScores = 2;

    public:

        bool running;

        ScoreText() {
            GetComponent<TransformComponent>()->LocalPosition() = {0,2,.01};
            AddComponent<TextRendererComponent>();
            GetComponent<TextRendererComponent>()->SetData("fonts/Arial.ttf", "Score: 0");
        }

        void Start() override {
            score = 0;
            elapsedTime = 0;
            running = true;
        }

        void Update(float deltaTime) override {

            if (!running) {
                return;
            }

            elapsedTime += deltaTime;

            if (score == 0) {
                if (elapsedTime < initialDelay) {
                    return;
                }
                else {
                    IncrementScore();
                    return;
                }
            }
            else {
                if (elapsedTime > initialDelay + score * delayBetweenScores) {
                    IncrementScore();
                }
            }


        }
 
        void IncrementScore() {
            RemoveComponent<TextRendererComponent>();
            score++;
            GetComponent<TransformComponent>()->LocalPosition() = { 0,2,.01 };
            AddComponent<TextRendererComponent>();
            GetComponent<TextRendererComponent>()->SetData("fonts/Arial.ttf", std::format("Score: {}", score));
        }

        void GameOver() {
            RemoveComponent<TextRendererComponent>();
            GetComponent<TransformComponent>()->LocalPosition() = {0,0,.01};
            GetComponent<TransformComponent>()->LocalScale() = {.5,.5,1};
            AddComponent<TextRendererComponent>();
            GetComponent<TextRendererComponent>()->SetData("fonts/Arial.ttf", std::format("Game Over. Final Score: {}", score));
        }

    };

}

#endif