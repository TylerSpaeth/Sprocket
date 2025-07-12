#include "QuadRendererComponent.h"

#include "Events/RenderEvent.h"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"

#include <stdexcept>
#include <iostream>

namespace Sprocket {

  QuadRendererComponent::QuadRendererComponent() : m_EventCallback(nullptr) {}
  QuadRendererComponent::QuadRendererComponent(const std::function<void(Event&)> eventCallback) : m_EventCallback(eventCallback) {}
  QuadRendererComponent::~QuadRendererComponent() {}

  void QuadRendererComponent::RenderNew(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    RenderNewEvent* e = new RenderNewEvent();
    m_EventCallback(*e);
    m_QuadID = e->GetQuadID();
    free(e);
  }

  void QuadRendererComponent::UpdateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

    RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::MODEL_MATRIX, m_QuadID);
    
    glm::mat4 tr = glm::translate(glm::mat4(1), position);
    glm::mat4 xrot = glm::rotate(glm::mat4(1), glm::radians(-rotation.x), glm::vec3(1,0,0));
    glm::mat4 yrot = glm::rotate(glm::mat4(1), glm::radians(-rotation.y), glm::vec3(0,1,0));
    glm::mat4 zrot = glm::rotate(glm::mat4(1), glm::radians(-rotation.z), glm::vec3(0,0,1));
    glm::mat4 rt = xrot*yrot*zrot;
    glm::mat4 sc = glm::scale(glm::mat4(1), scale);
    e->m_Matrix = tr * rt * sc;

    m_EventCallback(*e);

    free(e);
  }

  void QuadRendererComponent::RemoveRender() {
    RenderDeleteEvent* e = new RenderDeleteEvent(m_QuadID);
    m_EventCallback(*e);
    free(e);
  }

  void QuadRendererComponent::UpdateQuadColor(glm::vec4 newColor) {
    m_QuadColor = newColor;
    RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::QUAD, m_QuadID);
    e->m_QuadColor = m_QuadColor;
    m_EventCallback(*e);
    free(e);
  }
}