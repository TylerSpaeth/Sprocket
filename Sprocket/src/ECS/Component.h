#ifndef COMPONENT_H
#define COMPONENT_H

//#include "ECS/Entity.h"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

  enum class ComponentType {
    TRANSFORM_COMPONENT,
    TEST_COMPONENT
  };

  class Entity;

  class Component {
    private:
      ComponentType m_ComponentType;
    protected:
      Component(ComponentType componentType) : m_ComponentType(componentType){}
    public:
      ComponentType GetComponentType() const {return m_ComponentType;}
  };

  class TransformComponent : public Component {
    friend class Entity;
    private:
      TransformComponent() : Component(ComponentType::TRANSFORM_COMPONENT){}
    public:
      glm::vec3 m_Position = glm::vec3(0,0,0);
      glm::vec3 m_Rotation = glm::vec3(0,0,0);
      glm::vec3 m_Scale = glm::vec3(1,1,1);
  };

  class TestComponent : public Component {
    public:
      int value;
      TestComponent() : Component(ComponentType::TEST_COMPONENT), value(0) {}
      
  };

}

#endif