#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Sprocket.h"

namespace Sprocket {

  class Camera : public Entity {

  public:
    Entity* entityToFollow;

    Camera() {
      AddComponent<CameraComponent>();
    }
 
    void Update(float deltaTime) override {
      GetComponent<TransformComponent>()->LocalPosition() = entityToFollow->GetComponent<TransformComponent>()->LocalPosition();
    }

  };

}


#endif