#ifndef COLLISION_H
#define COLLISION_H

#include "ECS/Component.h"

namespace Sprocket {

  namespace Collision {
    
    /// @brief Determines if there was a collsion between two box colliders. Utilizes the Seperating
    /// Asix Theorem, so rotation works.
    /// @param b1 The first box collider.
    /// @param b1t The transform corresponding with the first box collider.
    /// @param b2 The second box collider.
    /// @param b2t The transform cooresponding with the second box collider.
    /// @return true if there is a collision, false otherwise
    bool Collides(BoxColliderComponent b1, TransformComponent b1t, BoxColliderComponent b2, TransformComponent b2t);

    /// @brief Determines if there was a collsion between two circle colliders. Note that circle
    /// colliders are scaled according to the x value only, so any other scaled dimensions in the 
    /// transform will not effect collision.
    /// @param c1 The first circle collider.
    /// @param c1t The transform corresponding with the first circle collider.
    /// @param c2 The second circle collider.
    /// @param c2t The transform cooresponding with the second circle collider.
    /// @return true if there is a collision, false otherwise
    bool Collides(CircleColliderComponent c1, TransformComponent c1t, CircleColliderComponent c2, TransformComponent c2t);

    /// @brief Determines if there was a collsion between a box and circle collider. Note that 
    /// circle colliders are scaled according to the x value only, so any other scaled dimensions 
    /// in the transform will not effect collision.
    /// @param b The box collider. 
    /// @param bt The transform corresponding to the box collider.
    /// @param c The circle collider.
    /// @param ct The transform corresponding to the circle collider.
    /// @return true if there is a collision, false otherwise
    bool Collides(BoxColliderComponent b, TransformComponent bt, CircleColliderComponent c, TransformComponent ct);

    /// @brief Determines if there was a collsion between a box and circle collider. Note that 
    /// circle colliders are scaled according to the x value only, so any other scaled dimensions 
    /// in the transform will not effect collision.
    /// @param c The circle collider.
    /// @param ct The transform corresponding to the circle collider.
    /// @param b The box collider. 
    /// @param bt The transform corresponding to the box collider.
    /// @return true if there is a collision, false otherwise
    bool Collides(CircleColliderComponent c, TransformComponent ct, BoxColliderComponent b, TransformComponent bt);
  }

}

#endif