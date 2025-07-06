#include "CameraComponent.h"

#include "Events/RenderEvent.h"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"

namespace Sprocket {

  void CameraComponent::UpdateCameraPosition(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

    RenderUpdateEvent* e = new RenderUpdateEvent(RenderUpdateType::VIEW_MATRIX);

    glm::mat4 tr = glm::translate(glm::mat4(1), -position);
    glm::mat4 xrot = glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1,0,0));
    glm::mat4 yrot = glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0,1,0));
    glm::mat4 zrot = glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0,0,1));
    glm::mat4 rt = xrot * yrot * zrot;
    glm::mat4 sc = glm::scale(glm::mat4(1), scale);

    e->m_Matrix = tr * rt * sc;
    m_EventCallback(*e);
    free(e);

  }

}