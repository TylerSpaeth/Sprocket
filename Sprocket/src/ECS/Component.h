#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Macros.h"

#include "ThirdParty/glm/glm.hpp"

#include <string>

namespace Sprocket {

  enum class ComponentType {
    DELETED_COMPONENT,
    TRANSFORM_COMPONENT,
    QUAD_RENDERER
  };

  struct Component {
    ComponentType componentType;
    bool modified = false; // Must be set true for modifications to be sent to systems
    bool initialized = false; 
  };

  struct TransformComponent : public Component {
    glm::vec3 position = glm::vec3(0,0,0);
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);
    TransformComponent() {
      componentType = ComponentType::TRANSFORM_COMPONENT;
      initialized = true; // For not transforms components do not need to be intialized
    }
  };

  struct QuadRendererComponent : public Component {
    unsigned int quadID;
    float size;
    std::string texturePath;
    glm::vec4 quadColor = {1,1,1,1};
    glm::vec4 quadXCoords = {1,1,0,0};
    glm::vec4 quadYCoords = {1,0,0,1};
    QuadRendererComponent() {componentType = ComponentType::QUAD_RENDERER;}
  };

}

#endif