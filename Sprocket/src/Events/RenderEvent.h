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
    QUAD_COLOR, 
    QUAD_TEX_COORDS, 
    QUAD_TEX
  };

  /// @brief This class is used to transfer rendering data through the event system. The various
  /// public variables describe different attributes that could be needed for different 
  /// RenderUpdateTypes.
  /// @details Below are the variables used by each RenderUpdateType:
  /// - MODEL_MATRIX
  ///   - m_QuadIndex and m_Matrix
  /// - VIEW_MATRIX
  ///   - m_Matrix
  /// - QUAD_COLOR
  ///   - m_QuadIndex and m_Vec1
  /// - QUAD_TEX_COORDS
  ///   - m_QuadIndex, m_Vec1, and m_Vec2
  /// - QUAD_TEX
  ///   - m_QuadIndex and m_TexturePath
  class RenderUpdateEvent : public Event {
    private:
      RenderUpdateType m_Type;

    public:
      unsigned int m_QuadIndex = -1;
      std::string m_TexturePath;
      // Used to store either view or model matrix depending on the RenderUpdateType
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