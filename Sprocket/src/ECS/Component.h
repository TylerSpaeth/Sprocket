#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Macros.h"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

  enum class ComponentType {
    TRANSFORM_COMPONENT,
    QUAD_RENDERER,
    TEST_COMPONENT
  };

  class Entity;

  class Component {
    friend class Entity;
    private:
      ComponentType m_ComponentType;
      virtual void OnAttach(){}
      virtual void OnDetach(){}
    protected:
      Component(ComponentType componentType) : m_ComponentType(componentType){}
      Entity* m_Entity;
    public:
      ComponentType GetComponentType() const {return m_ComponentType;}
  };

  class SPROCKET_API TransformComponent : public Component {
    friend class Entity;
    private:
      TransformComponent() : Component(ComponentType::TRANSFORM_COMPONENT){}
    public:
      glm::vec3 m_Position = glm::vec3(0,0,0);
      glm::vec3 m_Rotation = glm::vec3(0,0,0);
      glm::vec3 m_Scale = glm::vec3(1,1,1);
  };

  class SPROCKET_API QuadRenderer : public Component {
    friend class Entity;
    private:
      void OnAttach() override;
      void OnDetach() override;
      unsigned int m_QuadID = -1;
      float m_TextureID;
      float m_Size;
    public:
      QuadRenderer(float size, float textureID) : Component(ComponentType::QUAD_RENDERER), m_Size(size), m_TextureID(textureID){}
  };

  class SPROCKET_API TestComponent : public Component {
    public:
      int value;
      TestComponent() : Component(ComponentType::TEST_COMPONENT), value(0) {}
  };

}

#endif