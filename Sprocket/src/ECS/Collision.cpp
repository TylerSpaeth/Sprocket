#include "ECS/Collision.h"

#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"

#include <cmath>
#include <vector>
#include <limits>

#include <iostream>

namespace Sprocket {

  namespace Collision {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////// UTILITY FUNCTIONS //////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

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

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// COLLISION FUNCTIONS /////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    // Box-Box Collision
    bool Collides(BoxColliderComponent b1, TransformComponent b1t, BoxColliderComponent b2, TransformComponent b2t) {
      std::vector<glm::vec2> normals1 = CalculateNormals(b1, b1t);
      std::vector<glm::vec2> normals2 = CalculateNormals(b2, b2t);

      for(glm::vec2 normal : normals1) {
        auto pair1 = ProjectToAxis(b1, b1t, normal);
        auto pair2 = ProjectToAxis(b2, b2t, normal);

        // If there is a between the projections
        if(pair1.second < pair2.first || pair2.second < pair1.first) {
          return false;
        }
      }

      for(glm::vec2 normal : normals2) {
        auto pair1 = ProjectToAxis(b1, b1t, normal);
        auto pair2 = ProjectToAxis(b2, b2t, normal);

        // If there is a between the projections
        if(pair1.second < pair2.first || pair2.second < pair1.first) {
          return false;
        }
      }

      // If no gap is ever detected, return true because there is a collision
      return true;
    }

    // Circle-Circle Collision
    bool Collides(CircleColliderComponent c1, TransformComponent c1t, CircleColliderComponent c2, TransformComponent c2t) {

      float xOffset;
      float yOffset;
      float distance;

      // Calculate the distance between the centers of the circles
      xOffset = c1t.position.x - c2t.position.x;
      yOffset = c1t.position.y - c2t.position.y;
      distance = sqrt(pow(xOffset,2) + pow(yOffset,2));

      // If the distance between the circles is less than the sum of the radii, then there is a collision
      if(distance <= c1.radius*c1t.scale.x + c2.radius*c2t.scale.x) return true;

      return false;
    }

    //TODO verify that this all works properly with various scales and rotations 

    // Box-Circle Collision
    bool Collides(BoxColliderComponent b, TransformComponent bt, CircleColliderComponent c, TransformComponent ct) {

      auto verts = GetVerts(b,bt);

      // Iterate through edges
      for(int i = 0; i < 4; i++) {
        glm::vec2 p1 = verts[i];
        glm::vec2 p2 = verts[(i+1)%4]; // Loop around

        // Calculate the edge 
        glm::vec2 edge = p2-p1;

        // Calculate the vector from p1 to the center of the circle
        glm::vec2 cToP1 = glm::vec2(ct.position.x, ct.position.y) - p1;

        // Square the edge vector to normalize the edge
        float el2 = glm::dot(edge,edge);

        // Project the circles center onto the edge and clamp 
        float t = glm::dot(cToP1,edge) / el2;
        t = glm::clamp(t,0.0f,1.0f);

        // Find the closest point on the edge from the circle
        glm::vec2 closestPoint = p1 + t * edge;

        // Calculate the distance between the closest point at the center of the circle
        float distance = glm::length(glm::vec2(ct.position.x,ct.position.y)-closestPoint);

        // If the distance between the circles center and the closest point on the box is not 
        // greater than the scaled radius, then a collision occured
        if(distance <= c.radius*ct.scale.x) {
          return true;
        }

      }

      // Check if the center of the circle is inside of the box
      { 
        // Get the circle center position relative to the box's position
        glm::vec2 localCenter = ct.position - bt.position;

        // Calculate the angles needs to rotate the circle into the correct local position
        float cosAngle = cos(-ct.rotation.z);
        float sinAngle = sin(-ct.rotation.z);

        // Calculate the rotated center
        glm::vec2 rotatedCenter;
        rotatedCenter.x = localCenter.x * cosAngle - localCenter.y * sinAngle;
        rotatedCenter.y = localCenter.x * sinAngle + localCenter.y * cosAngle;

        // Check if the center falls within the bounds of the box
        if(rotatedCenter.x < b.width/2*bt.scale.x && rotatedCenter.x > -b.width/2*bt.scale.x && 
          rotatedCenter.y < b.height/2*bt.scale.y && rotatedCenter.y > -b.height/2*bt.scale.y) {
          return true;
        }
      }

      return false; // No collision found
    }

    // This function just allows the circle and box collider to be supplied in the opposite order
    bool Collides(CircleColliderComponent c, TransformComponent ct, BoxColliderComponent b, TransformComponent bt) {
      return Collides(b,bt,c,ct);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

  }
}