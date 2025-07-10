#ifndef COLLISION_H
#define COLLISION_H

#include "ThirdParty/glm/glm.hpp"

#include <vector>

namespace Sprocket {

  // NOTE that it is expected for the vertecies vector to be ordered CCW
  namespace Collision {
    
    // Circle-Circle
    bool Collides(glm::vec2 circleCenter1, float circleRadius1, glm::vec2 circleCenter2, float circleRadius2);
    // Box-Box
    bool Collides(std::vector<glm::vec2> boxVerts1, std::vector<glm::vec2> boxVerts2);
    // Box-Circle - NOTE that boxRotation is expected to be the rotation of the 2d box in degrees
    bool Collides(std::vector<glm::vec2> boxVerts, glm::vec2 boxPosition, float boxRotation, glm::vec2 circleCenter, float circleRadius);

  }

}

#endif