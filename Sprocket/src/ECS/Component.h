#ifndef COMPONENT_H
#define COMPONENT_H

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {
  
  class Component {

  };

  class TransformComponent : public Component {
    private:
      glm::vec3 m_Transform;
      glm::vec3 m_Rotation;
      glm::vec3 m_Scale;
  };

}

#endif