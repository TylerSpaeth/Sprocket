#ifndef COLLISION_H
#define COLLISION_H

#include "ECS/Component.h"
#include "Core/Macros.h"


namespace Sprocket {

  // The current implementation for collsion testing uses the Seperating Axis Theorem
  class SPROCKET_API Collision {
    private:
      // If the two colliders collide
      bool m_Collide = false;
    public:
      // Box-Box Collision
      Collision(BoxColliderComponent b1, TransformComponent b1t, BoxColliderComponent b2, TransformComponent b2t);
      // Circle-Circle Collision
      Collision(CircleColliderComponent c1, TransformComponent c1t, CircleColliderComponent c2, TransformComponent c2t);
      // Box-Circle Collision
      Collision(BoxColliderComponent b, TransformComponent bt, CircleColliderComponent c, TransformComponent ct);
      // Box-Circle Collision
      Collision(CircleColliderComponent c, TransformComponent ct, BoxColliderComponent b, TransformComponent bt) {
        Collision(b,bt,c,ct);
      }

      bool Collides() const {return m_Collide;}
  };

}

#endif