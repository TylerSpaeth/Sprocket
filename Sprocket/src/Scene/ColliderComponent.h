#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

namespace Sprocket {

  class ColliderComponent {
    public:
      // TODO determine what else should be passed to these functions. There needs to be a 
      // way to determine position, but we really do not want to pass in the transform component as
      // it would be less flexible then, but we also probably do not want to store the positions on 
      // these components as it would increase memory usage
      // On Second thought it may be best for each collider to have some sort of unique identifier,
      // perhaps just its address so that we can lookup collisions in a collidion table
      virtual bool CollidesWith(BoxColliderComponent& collider);
      virtual bool CollidesWith(CircleColliderComponent& collider);
  };

  class BoxColliderComponent : public ColliderComponent {

    private:
      float m_Width;
      float m_Height;

    public:
      float GetWidth() const;
      void SetWidth(float width);

      float GetHeight() const;
      void SetHeight(float height);

  };

  class CircleColliderComponent : public ColliderComponent {

    private:
      float m_Radius;

    public:
      float GetRadius() const;
      void SetRadius(float radius);

  };
}

#endif