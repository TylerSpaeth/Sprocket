#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Macros.h"

#include "ThirdParty/glm/glm.hpp"

#include <string>

namespace Sprocket {

  enum class ComponentType {
    DELETED_COMPONENT,
    TRANSFORM_COMPONENT,
    QUAD_RENDERER_COMPONENT,
    CAMERA_COMPONENT,
    BOX_COLLIDER_COMPONENT,
    CIRCLE_COLLIDER_COMPONENT,
    PHYSICS_COMPONENT
  };

  struct Component {
    const ComponentType componentType; // TODO we may be able to remove this
    Component(ComponentType type) : componentType(type){}
  };

  struct TransformComponent : public Component {
    glm::vec3 position = glm::vec3(0,0,0);
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);
    TransformComponent() : Component(ComponentType::TRANSFORM_COMPONENT){}
  };

  // Right now, with the way other systems are setup, size can not be change after the component is
  // added to an entity. 
  // TODO this is something that should be fixed. It would probably be best not to allow the user
  // to specify the size of individual quads, but instead specify a single quad size and then just 
  // use scale for quads that should be rendered as different sizes
  struct QuadRendererComponent : public Component {
    unsigned int quadID;
    float size;
    std::string texturePath; // Set to "" in order to use the quad color instead
    glm::vec4 quadColor = {1,1,1,1};
    glm::vec4 quadXCoords = {1,1,0,0};
    glm::vec4 quadYCoords = {1,0,0,1};
    QuadRendererComponent() : Component(ComponentType::QUAD_RENDERER_COMPONENT){}
  };

  struct CameraComponent : public Component {
    CameraComponent() : Component(ComponentType::CAMERA_COMPONENT){}
  };

  struct ColliderComponent : public Component {
    ColliderComponent(ComponentType type) : Component(type){}
  };

  struct BoxColliderComponent : public ColliderComponent {
    float width;
    float height;
    BoxColliderComponent() : ColliderComponent(ComponentType::BOX_COLLIDER_COMPONENT){}
  };

  struct CircleColliderComponent : public ColliderComponent {
    float radius;
    CircleColliderComponent() : ColliderComponent(ComponentType::CIRCLE_COLLIDER_COMPONENT){}
  };

  struct PhysicsComponent : public Component {
    // PhysicsComponents that are marked as dynamic are expected to move and is the default.
    // If set to false, then it is not expected to move.
    bool isDynamic = true;
    // This ID is used to reference a physic object in the physics system
    int phyiscsID = -1;
    PhysicsComponent() : Component(ComponentType::PHYSICS_COMPONENT){}
  };

}

#endif