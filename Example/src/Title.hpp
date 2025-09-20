#ifndef TITLE_HPP
#define TITLE_HPP

#include "Sprocket.h"

namespace Sprocket {

    class Title : public Entity {

    public:
        Title() {
            AddComponent<TextRendererComponent>();
            GetComponent<TextRendererComponent>()->SetData("fonts/Arial.ttf", "The quick brown fox jumps over the lazy dog.");
            auto transform = GetComponent<TransformComponent>();
            transform->LocalScale() = {0.5, 0.5, 1};
            transform->LocalPosition().y -= 2.5;
            transform->LocalPosition().z += .01;
        }

    };

}

#endif