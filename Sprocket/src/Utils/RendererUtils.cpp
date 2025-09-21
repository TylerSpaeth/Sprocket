#include "RendererUtils.h"

namespace Sprocket {

    namespace RendererUtils {

        /// @brief Calculates a transformation matrix from position, rotation, and scale vectors.
        /// @param position - The position vector.
        /// @param rotation - The rotation vector (in degrees).
        /// @param scale - The scale vector.
        glm::mat4 CalculateMatrixFromVecs(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
            glm::mat4 translate = glm::translate(glm::mat4(1), -position);
            glm::mat4 xrot = glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1, 0, 0));
            glm::mat4 yrot = glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0, 1, 0));
            glm::mat4 zrot = glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0, 0, 1));
            glm::mat4 rotate = xrot * yrot * zrot;
            glm::mat4 sc = glm::scale(glm::mat4(1), scale);

            return translate * rotate * sc;
        }

    }

}