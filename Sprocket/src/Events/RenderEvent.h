#ifndef RENDEREVENT_H
#define RENDEREVENT_H

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

#include "Core/Sprocket.pch"

namespace Sprocket {

    class RenderNewEvent : public Event {
        friend class Renderer;
    private:
        int m_QuadID = -1;
    public:
        RenderNewEvent() {
            this->SetType(EventType::RENDER_NEW);
        }

        const int GetQuadID() const { return m_QuadID; }
    };

    enum class RenderUpdateType {
        MODEL_MATRIX,
        VIEW_MATRIX,
        QUAD
    };

    // TODO make a seperate event for model matrix updates
    /// @brief This class is used to transfer rendering data through the event system. The various
    /// public variables describe different attributes that could be needed for different 
    /// RenderUpdateTypes.
    class RenderUpdateEvent : public Event {
        friend class Renderer;
        friend class EventValidation;
    private:
        RenderUpdateType m_Type;
        int m_QuadID = -1;

    public:
        std::string m_TexturePath; // A path of "" represents no texture and uses the quadcolor 
        // Used to store either view or model matrix depending on the RenderUpdateType
        glm::mat4 m_Matrix;
        glm::vec4 m_QuadColor = {1,1,1,1};
        // Texture coords start with the top right corner and go clockwise around the quad
        glm::vec4 m_TexXCoords;
        glm::vec4 m_TexYCoords;

        RenderUpdateEvent(RenderUpdateType type, const unsigned int quadID) : m_Type(type), m_QuadID(quadID) {
            this->SetType(EventType::RENDER_UPDATE);
        }

        const RenderUpdateType GetType() const { return m_Type; }

        const int GetQuadID() const { return m_QuadID; }
    };

    class RenderDeleteEvent : public Event {
        friend class Renderer;
    private:
        int m_QuadID = -1;
    public:
        RenderDeleteEvent(const unsigned int quadIndex) : m_QuadID(quadIndex) {
            this->SetType(EventType::RENDER_DELETE);
        }

        const int GetQuadID() const { return m_QuadID; }
    };

    class RenderNewTextEvent : public Event {
        friend class Renderer;
    private:
        int m_QuadID = -1;
        std::string m_FontPath;
        std::string m_Text;
    public:
        RenderNewTextEvent(const std::string& fontPath, const std::string& text) : m_FontPath(fontPath), m_Text(text) {
            this->SetType(EventType::RENDER_NEW_TEXT);
        }

        const int GetQuadID() const { return m_QuadID; }
    };

    class RenderUpdateTextColorEvent : public Event {
        friend class Renderer;
    private:
        int m_QuadID = -1;
        glm::vec4 m_QuadColor;
    public:
        RenderUpdateTextColorEvent(const unsigned int quadID, const glm::vec4 color) : m_QuadID(quadID) , m_QuadColor(color) {
            this->SetType(EventType::RENDER_UPDATE_TEXT_COLOR);
        }
    };

}

#endif