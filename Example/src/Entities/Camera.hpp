#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Sprocket.h"

namespace Sprocket {

    class Camera : public Entity {

    public:
        Entity* m_EntityToFollow = nullptr;

        Camera() {
            AddComponent<CameraComponent>();
        }

        void Update(float deltaTime) override {
            if (m_EntityToFollow != nullptr) {
                GetComponent<TransformComponent>()->LocalPosition() = m_EntityToFollow->GetComponent<TransformComponent>()->LocalPosition();
            }
        }

    };

}


#endif