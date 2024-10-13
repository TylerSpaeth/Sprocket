#ifndef RENDEREVENT_H
#define RENDEREVENT_H

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

  class RenderNewEvent : public Event {
    private:
      const float m_Size;
      const float m_TextureID;

    public:
      unsigned int m_QuadID = -1;
      RenderNewEvent(float size, float textureID) : m_Size(size), m_TextureID(textureID){
        this->SetType(EventType::RENDER_NEW);
      }
      float GetSize() const {return m_Size;}
      float GetTextureID() const {return m_TextureID;}
  };

  enum class RenderUpdateType {
    MODEL_MATRIX, 
    VIEW_MATRIX,
    QUAD_COLOR, 
    QUAD_TEX_COORDS, 
    QUAD_TEX_ID
  };

  // The various variables in this class can have different purposed depending on the 
  // RenderUpdateType of this event. 
  class RenderUpdateEvent : public Event {
    private:
      RenderUpdateType m_Type;

    public:
      unsigned int m_QuadIndex = -1;
      float m_TextureID = -1;
      // Used to store either quad or model matrix depending on the RenderUpdateType
      glm::mat4 m_Matrix;
      // Used to store the color for a QUAD_COLOR update, or xCoords for QUAD_TEX_COORDS
      glm::vec4 m_Vec1;
      // Used to store yCoords for QUAD_TEX_COORDS
      glm::vec4 m_Vec2;

      RenderUpdateEvent(RenderUpdateType type) : m_Type(type) {
        this->SetType(EventType::RENDER_UPDATE);
      }

      RenderUpdateType GetType() const {return m_Type;}
  };

}

#endif