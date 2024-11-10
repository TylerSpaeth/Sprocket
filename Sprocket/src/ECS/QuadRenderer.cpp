#include "ECS/QuadRenderer.h"

#include <stdexcept>

namespace Sprocket {

  void QuadRenderer::VerifyCallback() {
    if(m_EventCallback == nullptr) {
      throw std::runtime_error("QuadRenderer can not be used without an active event callback.");
    }
  }
  
  void QuadRenderer::RenderNewQuad(TransformComponent transform, QuadRendererComponent& renderer) {

    VerifyCallback();
        
    RenderNewEvent* event = new RenderNewEvent(renderer.size);
    m_EventCallback(*event);
    renderer.quadID = event->m_QuadID;

    SetModelMatrix(transform, renderer);
  }

  void QuadRenderer::SetModelMatrix(TransformComponent transform, QuadRendererComponent& renderer) {

    VerifyCallback();

    RenderUpdateEvent* update = new RenderUpdateEvent(RenderUpdateType::MODEL_MATRIX);
    update->m_QuadIndex = renderer.quadID;
    
    glm::mat4 translate = glm::translate(glm::mat4(1), transform.position);
    glm::mat4 xrot = glm::rotate(glm::mat4(1), glm::radians(-transform.rotation.x), glm::vec3(1,0,0));
    glm::mat4 yrot = glm::rotate(glm::mat4(1), glm::radians(-transform.rotation.y), glm::vec3(0,1,0));
    glm::mat4 zrot = glm::rotate(glm::mat4(1), glm::radians(-transform.rotation.z), glm::vec3(0,0,1));
    glm::mat4 rotation = xrot * yrot * zrot;
    glm::mat4 scale = glm::scale(glm::mat4(1), transform.scale);
    update->m_Matrix = translate * rotation * scale;

    m_EventCallback(*update);
  }

  void QuadRenderer::UpdateQuad(QuadRendererComponent& renderer) {

    VerifyCallback();

    RenderUpdateEvent* update = new RenderUpdateEvent(RenderUpdateType::QUAD);
    update->m_QuadIndex = renderer.quadID;
    update->m_QuadColor = renderer.quadColor;
    update->m_TexturePath = renderer.texturePath;
    update->m_TexXCoords = renderer.quadXCoords;
    update->m_TexYCoords = renderer.quadYCoords;
    m_EventCallback(*update);
  }

  void QuadRenderer::DeleteQuad(QuadRendererComponent& renderer) {

    VerifyCallback();

    RenderDeleteEvent* e = new RenderDeleteEvent(renderer.quadID);
    m_EventCallback(*e);
  }

}