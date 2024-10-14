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
      
      RenderNewEvent* event = new RenderNewEvent(m_Size, m_TextureID);
      callback(*event);
      m_QuadID = event->m_QuadID;
    
      RenderUpdateEvent* update = new RenderUpdateEvent(RenderUpdateType::MODEL_MATRIX);
      update->m_QuadIndex = m_QuadID;
      // TODO add rotation
      update->m_Matrix = glm::scale(glm::mat4(1), t.m_Scale)* glm::translate(glm::mat4(1), t.m_Position);
      callback(*update);
    }

  }

  void QuadRenderer::OnDetach() {
    
  }

}