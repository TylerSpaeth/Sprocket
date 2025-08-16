#ifndef QUADRENDERERCOMPONENT_H
#define QUADRENDERERCOMPONENT_H

#include "Component.h"

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

#include "Core/Sprocket.pch"

namespace Sprocket {

  /// @brief QuadRendererComponent are all fixed size squares. To modify the shape an size, utilize
  /// the TransformComponent of the Entity this is attached to.
  class SPROCKET_API QuadRendererComponent : public Component {

    friend class Entity;

    private:

      // This correspondes to the id in the rendering system. It is needed to do anything with 
      // the component in the rendering system.
      int m_QuadID = -1;
      glm::vec4 m_QuadColor = {1,1,1,1};
      std::function<void(Event&)> m_EventCallback;
      std::string m_TexturePath;
      // These coordinates start in the top right corner and go clockwise around the quad
      glm::vec4 m_TextureXUVCoords = {1,1,0,0};
      glm::vec4 m_TextureYUVCoords = {1,0,0,1};
      
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

      void SendTextureEvent();

    public:

      // NOTE currently calling either of these updates will cause the renderer to switch between 
      // color and texture. It should be determined whether this is correct behavior.
      glm::vec4 GetQuadColor() const {return m_QuadColor;}
      void UpdateQuadColor(glm::vec4 newColor);
      void UpdateTexturePath(std::string texturePath);
      void UpdateTexturePath(std::string texturePath, glm::vec4 textureXUVCoords, glm::vec4 textureYUVCoords);
      void UpdateTextureCoords(glm::vec4 textureXUVCoords, glm::vec4 textureYUVCoords);

  };

}

#endif