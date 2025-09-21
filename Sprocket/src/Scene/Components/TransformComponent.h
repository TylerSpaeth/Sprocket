#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Component.h"

#include "ThirdParty/glm/glm.hpp"

#include "Core/Sprocket.pch"

namespace Sprocket {
    
    /// @brief Component representing position, rotation, and scale of an Entity.
    class SPROCKET_API TransformComponent : public Component {

        friend class Entity;

    private:

        glm::vec3 m_LocalPosition = glm::vec3(0, 0, 0);
        glm::vec3 m_LocalRotation = glm::vec3(0, 0, 0);
        glm::vec3 m_LocalScale = glm::vec3(1, 1, 1);
        // Indicates whether the transform has been modified since the last time it was checked.
        bool m_Modified = false;

        /// @brief A callback function that returns the global transform of the entity this component is attached to.
        std::function<TransformComponent()> m_GlobalTransformCallback;

    public:

        TransformComponent(std::function<TransformComponent()> callback);

        /// @brief The local position of this TransformComponent.
        glm::vec3& LocalPosition();
        /// @brief The local rotation of this TransformComponent.
        glm::vec3& LocalRotation();
        /// @brief The local scale of this TransformComponent.
        glm::vec3& LocalScale();
        /// @brief The global position of this TransformComponent.
        glm::vec3 Position();
        /// @brief The global rotation of this TransformComponent.
        glm::vec3 Rotation();
        /// @brief The global scale of this TransformComponent.
        glm::vec3 Scale();

    };

}

#endif