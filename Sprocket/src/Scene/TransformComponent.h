#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Component.h"

#include "ThirdParty/glm/glm.hpp"

#include "Core/Sprocket.pch"

namespace Sprocket {

  class SPROCKET_API TransformComponent : public Component {

    friend class Entity;

    private:

      glm::vec3 m_LocalPosition = glm::vec3(0,0,0);
      glm::vec3 m_LocalRotation = glm::vec3(0,0,0);
      glm::vec3 m_LocalScale = glm::vec3(1,1,1);
      bool m_Modified = false;

      // A callback function provided by the entity that this component is instantiated on
      std::function<TransformComponent()> m_GlobalTransformCallback;

    public:

      TransformComponent(std::function<TransformComponent()> callback) : m_GlobalTransformCallback(callback){}

      glm::vec3& LocalPosition();
      glm::vec3& LocalRotation();
      glm::vec3& LocalScale();
      glm::vec3 Position();
      glm::vec3 Rotation();
      glm::vec3 Scale();

  };

}

#endif