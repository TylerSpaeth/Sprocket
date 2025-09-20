#ifndef ENTERTOSTART_H
#define ENTERTOSTART_H

#include "Sprocket.h"

namespace Sprocket {

    class EnterToStart : public Entity {

    public:
        EnterToStart() {
            AddComponent<TextRendererComponent>();
            GetComponent<TextRendererComponent>()->SetData("fonts/Arial.ttf", "Press Enter to Start");
            auto transform = GetComponent<TransformComponent>();
            transform->LocalScale() = { 0.5, 0.5, 1 };
            transform->LocalPosition().z += .01;
        }

        void Update(float deltaTime) override {

            if (Input::IsKeyPressed(KEY_ENTER)) {
                SceneManager::SetActiveScene(2);
            }

        }

    };

}

#endif