#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "Component.h"

#include "ThirdParty/glm/glm.hpp"

namespace Sprocket {

    /// @brief The main camera of the scene. Only add one to a scene at a time.
    class SPROCKET_API CameraComponent : public EventDrivenComponent {

        friend class Entity;

    private:

        CameraComponent() {}
        ~CameraComponent() {}
    
        /// @brief Updates the camera transform to the given position, rotation, and scale
        /// @param position 
        /// @param rotation
        /// @param scale
        void UpdateCameraTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    };

}

#endif