#include "QuadRendererComponent.h"

#include "Events/RenderEvent.h"

#include "Utils/RendererUtils.hpp"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"

#include <stdexcept>

namespace Sprocket {

  QuadRendererComponent::QuadRendererComponent() : m_EventCallback(nullptr) {}
  QuadRendererComponent::QuadRendererComponent(const std::function<void(Event&)> eventCallback) : m_EventCallback(eventCallback) {}
  QuadRendererComponent::~QuadRendererComponent() {}

  void QuadRendererComponent::RenderNew(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    RenderNewEvent* e = new RenderNewEvent();
    m_EventCallback(*e);
    m_QuadID = e->GetQuadID();
    delete e;
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
  }

  void QuadRendererComponent::UpdateQuadColor(glm::vec4 newColor) {
    m_QuadColor = newColor;
    RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::QUAD, m_QuadID);
    e->m_QuadColor = m_QuadColor;
    m_EventCallback(*e);
    delete e;
  }

  void QuadRendererComponent::SendTextureEvent() {
    RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::QUAD, m_QuadID);
    e->m_TexturePath = m_TexturePath;
    e->m_TexXCoords = m_TextureXUVCoords;
    e->m_TexYCoords = m_TextureYUVCoords;
    m_EventCallback(*e);
    delete e;
  }

  void QuadRendererComponent::UpdateTexturePath(std::string texturePath) {
    m_TexturePath = texturePath;
    if(!m_TexturePath.empty()) {
      SendTextureEvent();
    }
  }

  void QuadRendererComponent::UpdateTexturePath(std::string texturePath, glm::vec4 textureXUVCoords, glm::vec4 textureYUVCoords) {
    m_TexturePath = texturePath;
    m_TextureXUVCoords = textureXUVCoords;
    m_TextureYUVCoords = textureYUVCoords;
    if(!m_TexturePath.empty()) {
      SendTextureEvent();
    }
  }

  void QuadRendererComponent::UpdateTextureCoords(glm::vec4 textureXUVCoords, glm::vec4 textureYUVCoords) {
    m_TextureXUVCoords = textureXUVCoords;
    m_TextureYUVCoords = textureYUVCoords;
    if(!m_TexturePath.empty()) {
      SendTextureEvent();
    }
  }
}