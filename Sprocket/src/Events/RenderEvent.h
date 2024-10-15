#ifndef RENDEREVENT_H
#define RENDEREVENT_H

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

#include <string>

namespace Sprocket {

  class RenderNewEvent : public Event {
    private:
      const float m_Size;

    public:
      unsigned int m_QuadID = -1;
      RenderNewEvent(float size) : m_Size(size){
        this->SetType(EventType::RENDER_NEW);
      }
      float GetSize() const {return m_Size;}
  };

  enum class RenderUpdateType {
    MODEL_MATRIX, 
    VIEW_MATRIX,
    QUAD
  };

  /// @brief This class is used to transfer rendering data through the event system. The various
  /// public variables describe different attributes that could be needed for different 
  /// RenderUpdateTypes.
  class RenderUpdateEvent : public Event {
    private:
      RenderUpdateType m_Type;

    public:
      unsigned int m_QuadIndex = -1;
      std::string m_TexturePath;
      // Used to store either view or model matrix depending on the RenderUpdateType
      glm::mat4 m_Matrix;
      glm::vec4 m_QuadColor;
      glm::vec4 m_TexXCoords;
      glm::vec4 m_TexYCoords;

      RenderUpdateEvent(RenderUpdateType type) : m_Type(type) {
        this->SetType(EventType::RENDER_UPDATE);
      }

      RenderUpdateType GetType() const {return m_Type;}
  };

}

#endif