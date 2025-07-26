#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Sprocket.h"

namespace Sprocket {

  class Camera : public Entity {

  public:
    Entity* entityToFollow;

    void Start() override {
      this->AddComponent<CameraComponent>();
    }
 
    void Update(float deltaTime) override {
      this->GetComponent<TransformComponent>()->LocalPosition() = entityToFollow->GetComponent<TransformComponent>()->LocalPosition();
    }

  };

}


#endif