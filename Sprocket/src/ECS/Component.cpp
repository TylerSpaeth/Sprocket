#include "ECS/Component.h"

#include "ECS/Entity.h"
#include "Events/RenderEvent.h"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"

#include <iostream>

namespace Sprocket {

  void QuadRenderer::OnAttach() {
    TransformComponent t = m_Entity->GetGlobalTransform();

    // TODO a more efficient way for this should be found 
    // It would probably be best to have the callback stored in each entity that can 
    // be set in an on attach function and then set to nullptr with an ondetach function.
    // Otherwise this tree traversal may get expensive if there is component instantiation during
    // application runtime

    Entity* parent = m_Entity;
    while(!parent->IsRoot()) {
      parent = (Entity*)parent->GetParent();
    }

    auto callback = ((RootEntity*)parent)->GetEventCallback();

    if(callback != nullptr) {
      
      RenderNewEvent* event = new RenderNewEvent(m_Size);
      callback(*event);
      m_QuadID = event->m_QuadID;
    
      RenderUpdateEvent* update = new RenderUpdateEvent(RenderUpdateType::MODEL_MATRIX);
      update->m_QuadIndex = m_QuadID;

      glm::mat4 translate = glm::translate(glm::mat4(1), t.m_Position);
      glm::mat4 xrot = glm::rotate(glm::mat4(1), glm::radians(-t.m_Rotation.x), glm::vec3(1,0,0));
      glm::mat4 yrot = glm::rotate(glm::mat4(1), glm::radians(-t.m_Rotation.y), glm::vec3(0,1,0));
      glm::mat4 zrot = glm::rotate(glm::mat4(1), glm::radians(-t.m_Rotation.z), glm::vec3(0,0,1));
      glm::mat4 rotation = xrot * yrot * zrot;
      glm::mat4 scale = glm::scale(glm::mat4(1), t.m_Scale);
      
      update->m_Matrix = translate * rotation * scale;
      callback(*update);
    }

  }

  void QuadRenderer::OnDetach() {
    
  }

  void QuadRenderer::SetTexture(const std::string& path) {
    RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::QUAD_TEX);
    e->m_QuadIndex = m_QuadID;
    e->m_TexturePath = path;

    Entity* parent = m_Entity;
    while(!parent->IsRoot()) {
      parent = (Entity*)parent->GetParent();
    }

    auto callback = ((RootEntity*)parent)->GetEventCallback();

    if(callback != nullptr) {
      callback(*e);
    }

  }

}