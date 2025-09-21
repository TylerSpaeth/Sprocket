#ifndef RENDERERUTILS_H
#define RENDERERUTILS_H

#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"

namespace Sprocket {

    namespace RendererUtils {

        /// @brief Calculates a transformation matrix from position, rotation, and scale vectors.
        /// @param position - The position vector.
        /// @param rotation - The rotation vector (in degrees).
        /// @param scale - The scale vector.
        glm::mat4 CalculateMatrixFromVecs(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    }

}

#endif