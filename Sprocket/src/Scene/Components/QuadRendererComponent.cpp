#include "QuadRendererComponent.h"

#include "Core/Global.h"

#include "Events/RenderEvent.h"

#include "Utils/RendererUtils.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    glm::vec4 QuadRendererComponent::GetQuadColor() const {
        return m_QuadColor;
    }

    void QuadRendererComponent::SetQuadColor(glm::vec4 newColor) {
        m_QuadColor = newColor;
        RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::QUAD, m_QuadID);
        e->m_QuadColor = m_QuadColor;
        m_EventCallback(*e);
        delete e;
    }

    Sprite QuadRendererComponent::GetSprite() const {
        return m_Sprite;
    }

    void QuadRendererComponent::SetSprite(const Sprite& sprite) {
        m_Sprite = sprite;
        SendTextureEvent();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    QuadRendererComponent::QuadRendererComponent() {
        m_EventCallback = nullptr;
    }
    QuadRendererComponent::~QuadRendererComponent() {
        if (m_EventCallback && m_QuadID != -1) {
            RemoveRender();
        }
    }

    void QuadRendererComponent::RenderNew() {
        RenderNewEvent* e = new RenderNewEvent();
        m_EventCallback(*e);
        m_QuadID = e->GetQuadID();
        delete e;
        
        // Zero out everything to begin with so that it doesn't show up until values are assigned
        UpdateModelMatrix({0,0,0}, { 0,0,0 }, { 0,0,0 });
    }

    void QuadRendererComponent::UpdateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

        RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::MODEL_MATRIX, m_QuadID);

        // TODO investigate why the position and rotation need to be negative here. This behavior should
        // be the same with all model matricies.
        e->m_Matrix = RendererUtils::CalculateMatrixFromVecs(-position, -rotation, scale);

        m_EventCallback(*e);

        delete e;
    }

    void QuadRendererComponent::RemoveRender() {
        RenderDeleteEvent* e = new RenderDeleteEvent(m_QuadID);
        m_EventCallback(*e);
        delete e;
        m_QuadID = -1;
    }

    void QuadRendererComponent::SendTextureEvent() {

        if (m_Sprite.texturePath.empty()) {
            Global::fileLogger.Warning("The texturePath is empty. Failed to send texture event.");
            return;
        }

        RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::QUAD, m_QuadID);
        e->m_TexturePath = m_Sprite.texturePath;
        e->m_TexXCoords = m_Sprite.textureXUVCoords;
        e->m_TexYCoords = m_Sprite.textureYUVCoords;
        m_EventCallback(*e);
        delete e;
    }

    void QuadRendererComponent::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
        m_EventCallback = eventCallback;
        RenderNew();
    }
}