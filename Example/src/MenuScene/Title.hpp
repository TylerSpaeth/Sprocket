#ifndef TITLE_HPP
#define TITLE_HPP

#include "Sprocket.h"

namespace Sprocket {

    class Title : public Entity {

    public:
        Title() {
            AddComponent<TextRendererComponent>();
            auto transform = GetComponent<TransformComponent>();
            transform->LocalScale() = { 0.5, 0.5, 1 };
            transform->LocalPosition().y += 2;
            transform->LocalPosition().z += .01;
        }

        void Start() override {
            GetComponent<TextRendererComponent>()->SetData("fonts/Arial.ttf", "Test Application Title Screen");
        }

    };

}

#endif