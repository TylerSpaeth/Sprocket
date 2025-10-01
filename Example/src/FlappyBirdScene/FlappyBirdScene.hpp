#ifndef FLAPPYBIRDSCENE_HPP
#define FLAPPYBIRDSCENE_HPP

#include "Sprocket.h"
#include "Entities/Camera.hpp"
#include "ObstacleSpawner.hpp"
#include "Bird.hpp"
#include "ScoreText.hpp"
#include "Border.hpp"

namespace Sprocket {
    
    class FlappyBirdScene : public Scene {

    private:
        std::shared_ptr<Bird> bird;
        std::shared_ptr<ScoreText> scoreText;

    public:
        FlappyBirdScene() {
            SubmitEntityToScene(std::make_shared<Camera>());
            auto spawner = std::make_shared<ObstacleSpawner>();
            spawner->InitSelf(spawner);
            SubmitEntityToScene(spawner);
            bird = std::make_shared<Bird>();
            bird->gameOverCallback = std::bind(&FlappyBirdScene::GameOver, this);
            bird->GetComponent<TransformComponent>()->LocalPosition().x -= 3;
            SubmitEntityToScene(bird);
            scoreText = std::make_shared<ScoreText>();
            SubmitEntityToScene(scoreText);
            auto border = std::make_shared<Border>();
            border->GetComponent<TransformComponent>()->LocalPosition().y = 3.5;
            SubmitEntityToScene(border);
            border = std::make_shared<Border>();
            border->GetComponent<TransformComponent>()->LocalPosition().y = -3.5;
            SubmitEntityToScene(border);
        }

        void GameOver() {
            ((ScoreText*)scoreText.get())->GameOver();
            scoreText->running = false;
            RemoveEntityFromScene(bird);
        }

    };

}

#endif