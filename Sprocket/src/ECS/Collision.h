#ifndef COLLISION_H
#define COLLISION_H

#include "ECS/Component.h"
#include "Core/Macros.h"


namespace Sprocket {

  namespace Collision {
    bool Collides(BoxColliderComponent b1, TransformComponent b1t, BoxColliderComponent b2, TransformComponent b2t);
    bool Collides(CircleColliderComponent c1, TransformComponent c1t, CircleColliderComponent c2, TransformComponent c2t);
    // TODO implement these collision types
    //bool Collides(BoxColliderComponent b, TransformComponent bt, CircleColliderComponent c, TransformComponent ct);
    //bool Collides(CircleColliderComponent c, TransformComponent ct, BoxColliderComponent b, TransformComponent bt) {
      //return Collides(b,bt,c,ct);
    //}
  }

}

#endif