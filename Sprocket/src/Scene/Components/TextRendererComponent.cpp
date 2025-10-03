#include "TextRendererComponent.h"

#include "Events/RenderEvent.h"

#include "Utils/RendererUtils.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    TextRendererComponent::TextRendererComponent() {
        m_EventCallback = nullptr;
    }

    TextRendererComponent::~TextRendererComponent() {
        if (m_EventCallback && m_QuadID != -1) {
            RemoveRender();
        }
    }

    const bool TextRendererComponent::SetData(const std::string& fontPath, const std::string& text) {
        if (!m_FontPath.empty() && m_Text.empty()) {
            return false;
        }
        m_FontPath = fontPath;
        m_Text = text;

        if (m_EventCallback) {
            RenderNew();
        }

        return true;
    }

    const std::string TextRendererComponent::GetFontPath() const {
        return m_FontPath;
    }

    const std::string TextRendererComponent::GetText() const {
        return m_Text;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void TextRendererComponent::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
        m_EventCallback = eventCallback;

        if (!m_FontPath.empty() && !m_Text.empty()) {
            RenderNew();
        }
    }

    void TextRendererComponent::RenderNew() {
        RenderNewTextEvent* e = new RenderNewTextEvent(m_FontPath, m_Text);
        m_EventCallback(*e);
        m_QuadID = e->GetQuadID();
        delete e;
    }

    void TextRendererComponent::UpdateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

        RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::MODEL_MATRIX, m_QuadID);

        // TODO investigate why the position and rotation need to be negative here. This behavior should
        // be the same with all model matricies.
        e->m_Matrix = RendererUtils::CalculateMatrixFromVecs(-position, -rotation, scale);

        m_EventCallback(*e);

        delete e;
    }

    void TextRendererComponent::RemoveRender() {
        if (m_EventCallback && m_QuadID != -1) {
            RenderDeleteEvent* e = new RenderDeleteEvent(m_QuadID);
            m_EventCallback(*e);
            delete e;
        }
    }

}