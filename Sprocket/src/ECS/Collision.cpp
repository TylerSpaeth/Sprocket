#include "ECS/Collision.h"

#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"

#include <cmath>
#include <vector>
#include <limits>

#include <iostream>

namespace Sprocket {

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////// UTILITY FUNCTIONS ///////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  std::vector<glm::vec2> CalculateNormals(const BoxColliderComponent& b, const TransformComponent& t) {

    std::vector<glm::vec2> ret;

    for(int i = 0; i < 4; i++) {
      glm::vec2 normal = {cos(glm::radians(-t.rotation.z+i*90)),-sin(glm::radians(-t.rotation.z+i*90))};
      ret.push_back(normal);
    }
    
    return ret;
  }

  std::vector<glm::vec2> GetVerts(const BoxColliderComponent& b, const TransformComponent& t) {

    glm::mat4 translate = glm::translate(glm::mat4(1), t.position);
    glm::mat4 zrot = glm::rotate(glm::mat4(1), glm::radians(-t.rotation.z), glm::vec3(0,0,1));
    glm::mat4 scale = glm::scale(glm::mat4(1), t.scale);

    glm::mat4 transform = translate * zrot * scale;

    glm::vec2 v1 = transform * glm::vec4(b.width/2, b.height/2,0,1);
    glm::vec2 v2 = transform * glm::vec4(b.width/2, -b.height/2,0,1);
    glm::vec2 v3 = transform * glm::vec4(-b.width/2, -b.height/2,0,1);
    glm::vec2 v4 = transform * glm::vec4(-b.width/2, +b.height/2,0,1);

    return {v1,v2,v3,v4};
  }

  // Returns the min and maximum value of the box collider along the given axis
  std::pair<float, float> ProjectToAxis(const BoxColliderComponent& b, const TransformComponent& t, const glm::vec2& axis) {

    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

    for(glm::vec2 vert : GetVerts(b,t)) {
      float projection = glm::dot(vert,axis);
      min = std::min(min, projection);
      max = std::max(max, projection);
    }

    return {min,max};

  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  
  // Box-Box Collision
  Collision::Collision(BoxColliderComponent b1, TransformComponent b1t, BoxColliderComponent b2, TransformComponent b2t) {
    std::vector<glm::vec2> normals1 = CalculateNormals(b1, b1t);
    std::vector<glm::vec2> normals2 = CalculateNormals(b2, b2t);

    for(glm::vec2 normal : normals1) {
      auto pair1 = ProjectToAxis(b1, b1t, normal);
      auto pair2 = ProjectToAxis(b2, b2t, normal);

      // If there is a between the projections
      if(pair1.second < pair2.first || pair2.second < pair1.first) {
        m_Collide = false;
        return;
      }
    }

     for(glm::vec2 normal : normals2) {
      auto pair1 = ProjectToAxis(b1, b1t, normal);
      auto pair2 = ProjectToAxis(b2, b2t, normal);

      // If there is a between the projections
      if(pair1.second < pair2.first || pair2.second < pair1.first) {
        m_Collide = false;
        return;
      }
    }

    // If no gap is ever detected, return true because there is a collision
    m_Collide = true;
  }

  // Circle-Circle Collision
  Collision::Collision(CircleColliderComponent c1, TransformComponent c1t, CircleColliderComponent c2, TransformComponent c2t) {

    float xOffset;
    float yOffset;
    float distance;

    // Calculate the distance between the centers of the circles
    xOffset = c1t.position.x - c2t.position.x;
    yOffset = c1t.position.y - c2t.position.y;
    distance = sqrt(pow(xOffset,2) + pow(yOffset,2));

    // If the distance between the circles is less than the sum of the radii, then there is a collision
    if(distance < c1.radius + c2.radius) m_Collide = true;
  }

  // Box-Circle Collision
  Collision::Collision(BoxColliderComponent b, TransformComponent bt, CircleColliderComponent c, TransformComponent ct) {

    // TODO complete this kind of collision detection
  }

}