#ifndef COLLISION_H
#define COLLISION_H

#include "ECS/Component.h"

namespace Sprocket {

  namespace Collision {

    // Utilized the Seperating Axis Theorem, so the boxs can be rotated and detection still works
    bool Collides(BoxColliderComponent b1, TransformComponent b1t, BoxColliderComponent b2, TransformComponent b2t);

    // NOTE that circle colliders are scaled only according the x value of the transform, with the 
    // others not mattering

    bool Collides(CircleColliderComponent c1, TransformComponent c1t, CircleColliderComponent c2, TransformComponent c2t);
    bool Collides(BoxColliderComponent b, TransformComponent bt, CircleColliderComponent c, TransformComponent ct);
    bool Collides(CircleColliderComponent c, TransformComponent ct, BoxColliderComponent b, TransformComponent bt);
  }

}

#endif