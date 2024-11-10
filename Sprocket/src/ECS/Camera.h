#ifndef CAMERA_H
#define CAMERA_H

#include "Events/Event.h"
#include "Events/RenderEvent.h"
#include "ECS/Component.h"

#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"

#include <functional>
#include <stdexcept>

namespace Sprocket {

  class Camera {

    friend class Scene;

    private:

      Camera(){}

      std::function<void(Event&)> m_EventCallback;

      void VerifyCallback() {
        if(m_EventCallback == nullptr) {
          throw std::runtime_error("The Camera system can not be used without an active event callback.");
        }
      }
    
    public:

      /// @brief Sets the position, rotation, and scale of the camera to the given transform
      /// @param transform the new transform for the camera
      void UpdateCameraPosition(TransformComponent transform) {

        VerifyCallback();

        RenderUpdateEvent* update = new RenderUpdateEvent(RenderUpdateType::VIEW_MATRIX);
        
        glm::mat4 translate = glm::translate(glm::mat4(1), -transform.position);

        glm::mat4 xrot = glm::rotate(glm::mat4(1), glm::radians(transform.rotation.x), glm::vec3(1,0,0));
        glm::mat4 yrot = glm::rotate(glm::mat4(1), glm::radians(transform.rotation.y), glm::vec3(0,1,0));
        glm::mat4 zrot = glm::rotate(glm::mat4(1), glm::radians(transform.rotation.z), glm::vec3(0,0,1));
        glm::mat4 rotation = xrot * yrot * zrot;

        glm::mat4 scale = glm::scale(glm::mat4(1), transform.scale);

        update->m_Matrix = translate * rotation * scale;

        m_EventCallback(*update);
      }
  };

}

#endif