#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Sprocket.h"

namespace Sprocket {

  class Camera : public Entity {

  public:
    Entity* entityToFollow;

    void Start() override {
      AddComponent<CameraComponent>();
    }
 
    void Update(float deltaTime) override {
      GetComponent<TransformComponent>()->LocalPosition() = entityToFollow->GetComponent<TransformComponent>()->LocalPosition();
    }

  };

}


#endif