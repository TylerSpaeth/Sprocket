#ifndef FLAPPYBIRDSCENE_HPP
#define FLAPPYBIRDSCENE_HPP

#include "Sprocket.h"
#include "Entities/Camera.hpp"
#include "ObstacleSpawner.hpp"
#include "Bird.hpp"
#include "ScoreText.hpp"
#include "Border.hpp"
#include "GameOverSoundEffect.hpp"

namespace Sprocket {
    
    class FlappyBirdScene : public Scene {

    private:
        std::shared_ptr<Bird> bird;
        std::shared_ptr<ScoreText> scoreText;

    public:
        FlappyBirdScene() {

            SubmitEntityToScene<Camera>();
            auto spawner = SubmitEntityToScene<ObstacleSpawner>();
            bird = SubmitEntityToScene<Bird>();
            bird->gameOverCallback = std::bind(&FlappyBirdScene::GameOver, this);
            bird->GetComponent<TransformComponent>()->LocalPosition().x -= 3;
            auto border1 = SubmitEntityToScene<Border>();
            border1->GetComponent<TransformComponent>()->LocalPosition().y = 3.5;
            auto border2 = SubmitEntityToScene<Border>();
            border2->GetComponent<TransformComponent>()->LocalPosition().y = -3.5;
            scoreText = SubmitEntityToScene<ScoreText>();

        }

        void GameOver() {
            ((ScoreText*)scoreText.get())->GameOver();
            scoreText->running = false;
            RemoveEntityFromScene(bird);
            SubmitEntityToScene<GameOverSoundEffect>();
        }

    };

}

#endif