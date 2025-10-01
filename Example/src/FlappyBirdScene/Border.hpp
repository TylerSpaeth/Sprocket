#ifndef BORDER_HPP
#define BORDER_HPP

#include "Sprocket.h"

namespace Sprocket {

    class Border : public Entity {

    public:

        Border() {
            AddComponent<BoxColliderComponent>();
            GetComponent<TransformComponent>()->LocalScale().x = 20;
        }

    };

}

#endif
