#ifndef QUADRENDERERCOMPONENT_H
#define QUADRENDERERCOMPONENT_H

#include "Component.h"

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

#include <functional>

namespace Sprocket {

  /// @brief QuadRendererComponent are all fixed size squares. To modify the shape an size, utilize
  /// the TransformComponent of the Entity this is attached to.
  class SPROCKET_API QuadRendererComponent : public Component {

    friend class Entity;

    private:

      // This correspondes to the id in the rendering system. It is needed to do anything with 
      // the component in the rendering system.
      int m_QuadID = -1;

      std::function<void(Event&)> m_EventCallback;
      
      QuadRendererComponent();
      // The event callback should be pulled off of the scene that the entity is a part of
      QuadRendererComponent(const std::function<void(Event&)> eventCallback);
      ~QuadRendererComponent();

      /// @brief Submits this quad renderer into the rendering system with the given transform
      /// @param position 
      /// @param rotation 
      /// @param scale 
      void RenderNew(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
      /// @brief Updates the model matrix of this quad renderer based on the given transform
      /// @param position 
      /// @param rotation 
      /// @param scale 
      void UpdateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
      /// @brief Pulls this quad renderer out of the rendering system
      void RemoveRender();

    public:

      glm::vec4 m_QuadColor = {1,1,1,1};

  };

}

#endif