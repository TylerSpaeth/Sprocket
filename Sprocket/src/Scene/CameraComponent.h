#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Component.h"

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

#include <functional>

namespace Sprocket {

  /// @brief The main camera of the scene. Only add one to a scene at a time.
  class SPROCKET_API CameraComponent : public Component {

    friend class Entity;

    private:

      CameraComponent(){}
      ~CameraComponent(){}

      std::function<void(Event&)> m_EventCallback;

      void UpdateCameraPosition(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

  };

}

#endif