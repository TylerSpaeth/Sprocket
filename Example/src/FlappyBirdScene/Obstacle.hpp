#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include "Sprocket.h"

namespace Sprocket {

    class Obstacle : public Entity {

    private:

        float m_Speed = 2;
        float m_DistanceTraveled = 0;
        float m_DestroyAtDistance = 15;

    public:

        Obstacle() {
            AddComponent<QuadRendererComponent>();
            AddComponent<BoxColliderComponent>();
            GetComponent<TransformComponent>()->LocalScale().y = 4;
        }

        void Update(float deltaTime) override {
            
            float move = m_Speed * deltaTime;

            GetComponent<TransformComponent>()->LocalPosition().x -= move;
            
            m_DistanceTraveled += move;

            if(m_DistanceTraveled > m_DestroyAtDistance) {
                SceneManager::GetActiveScene()->RemoveEntityFromScene(m_Self);
            }

        }

    };

}

#endif