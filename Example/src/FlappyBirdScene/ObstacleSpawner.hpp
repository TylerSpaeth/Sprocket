#ifndef OBSTACLESPAWNER_HPP
#define OBSTACLESPAWNER_HPP

#include "Sprocket.h"
#include "Obstacle.hpp"

#include <random>

namespace Sprocket {

    class ObstacleSpawner : public Entity {

    private:

        float m_ElapsedTime;
        float m_Interval = 2;

        std::random_device rd;
        std::mt19937 gen;

    public:

        ObstacleSpawner() {
            GetComponent<TransformComponent>()->LocalPosition().x = 6.5;
            gen = std::mt19937(rd());
        }

        void Start() override {
            m_ElapsedTime = m_Interval;
        }
 
        void Update(float deltaTime) override {

            m_ElapsedTime += deltaTime;

            if (m_ElapsedTime >= m_Interval) {

                std::uniform_real_distribution<float> heightdis(-1.7f, 1.7f);
                float height = heightdis(gen);

                std::uniform_real_distribution<float> rdis(0,1);
                std::uniform_real_distribution<float> gdis(0,1);
                std::uniform_real_distribution<float> bdis(0,1);
                float red = rdis(gen);
                float green = gdis(gen);
                float blue = bdis(gen);
                glm::vec4 color = {red ,green, blue, 1};
                
                std::shared_ptr<Entity> obstacle1 = std::make_shared<Obstacle>();
                obstacle1->InitSelf(obstacle1);
                SceneManager::GetActiveScene()->SubmitEntityToScene(obstacle1);
                SceneManager::GetActiveScene()->AssignEntityParent(obstacle1, m_Self.lock());
                obstacle1->GetComponent<TransformComponent>()->LocalPosition().y = 3+height;
                obstacle1->GetComponent<QuadRendererComponent>()->SetQuadColor(color);
                std::shared_ptr<Entity> obstacle2 = std::make_shared<Obstacle>();
                obstacle2->InitSelf(obstacle2);
                SceneManager::GetActiveScene()->SubmitEntityToScene(obstacle2);
                SceneManager::GetActiveScene()->AssignEntityParent(obstacle2, m_Self.lock());
                obstacle2->GetComponent<TransformComponent>()->LocalPosition().y = -3+height;
                obstacle2->GetComponent<QuadRendererComponent>()->SetQuadColor(color);
                m_ElapsedTime = 0;
            }

        }

    };

}

#endif