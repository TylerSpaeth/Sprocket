#include "Collision.h"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"

namespace Sprocket {

    /// @brief Calculates the normals of the box. Requires the verts to ordered CCW
    /// @param verts 
    /// @return 
    std::array<glm::vec2, 4> CalculateNormals(std::array<glm::vec2, 4> verts) {

        std::array<glm::vec2, 4> normals;

        for (int i = 0; i < verts.size(); i++) {
            glm::vec2 edge = verts[(i + 1) % verts.size()] - verts[i];
            glm::vec2 normal = { -edge.y, edge.x };
            normal = glm::normalize(normal);
            normals.at(i) = normal;
        }
        return normals;
    }

    // Returns the min and maximum value of the box along the given axis
    std::pair<float, float> ProjectToAxis(std::array<glm::vec2, 4> verts, const glm::vec2& axis) {
        float min = std::numeric_limits<float>::max();
        float max = std::numeric_limits<float>::min();

        for (glm::vec2 vert : verts) {
            float projection = glm::dot(vert, axis);
            min = std::min(min, projection);
            max = std::max(max, projection);
        }

        return { min,max };
    }

    // TODO this should be redone to account for ovals
    bool Collision::Collides(glm::vec2 circleCenter1, float circleRadius1, glm::vec2 circleCenter2, float circleRadius2) {

        float xOffset = circleCenter1.x - circleCenter2.x;
        float yOffset = circleCenter1.y - circleCenter2.y;
        float distance = sqrt(pow(xOffset, 2) + pow(yOffset, 2));

        if (distance <= circleRadius1 + circleRadius2) return true;
        return false;
    }

    bool Collision::Collides(std::array<glm::vec2, 4> boxVerts1, std::array<glm::vec2, 4> boxVerts2) {

        auto normals1 = CalculateNormals(boxVerts1);
        auto normals2 = CalculateNormals(boxVerts2);

        for (glm::vec2 normal : normals1) {
            auto pair1 = ProjectToAxis(boxVerts1, normal);
            auto pair2 = ProjectToAxis(boxVerts2, normal);

            // If there is a gap between the projections
            if (pair1.second < pair2.first || pair2.second < pair1.first) {
                return false;
            }
        }

        for (glm::vec2 normal : normals2) {
            auto pair1 = ProjectToAxis(boxVerts1, normal);
            auto pair2 = ProjectToAxis(boxVerts2, normal);

            // If there is a gap between the projections
            if (pair1.second < pair2.first || pair2.second < pair1.first) {
                return false;
            }
        }

        // If no gap is ever detected, return true because there is a collision
        return true;
    }

    bool Collision::Collides(std::array<glm::vec2, 4> boxVerts, glm::vec2 boxPosition, float boxRotation, glm::vec2 circleCenter, float circleRadius) {

        // Iterate through the verts
        for (int i = 0; i < 4; i++) {
            glm::vec2 p1 = boxVerts[i];
            glm::vec2 p2 = boxVerts[(i + 1) % 4];

            glm::vec2 edge = p2 - p1;

            // Calculate the vector from p1 to the center of the circle
            glm::vec2 cToP1 = circleCenter - p1;

            // Square the edge vector to normalize the edge
            float el2 = glm::dot(edge, edge);

            // Project the circles center onto the edge and clamp
            float t = glm::dot(cToP1, edge) / el2;
            t = glm::clamp(t, 0.0f, 1.0f);

            // Find the closest point on the edge from the circle
            glm::vec2 closestPoint = p1 + t * edge;

            // Calculate the distance between the closest point at the center of the circle
            float distance = glm::length(circleCenter - closestPoint);

            // If the distance between the center and the closest point on the box is not greater than
            // the scaled radius, then a collision occured
            if (distance <= circleRadius) {
                return true;
            }
        }

        // Check if the center of the circle is inside of the box
        {

            // Calculate the relative positions of the points
            glm::vec2 relativeCenter = circleCenter - boxPosition;
            std::array<glm::vec2, 4> relativeVerts;
            for (int i = 0; i < boxVerts.size(); i++) {
                relativeVerts.at(i) = boxVerts.at(i) - boxPosition;
            }

            // Calculate the angles needed to rotate the points into the correct relative positions
            float cosAngle = cos(glm::radians(-boxRotation));
            float sinAngle = sin(glm::radians(-boxRotation));

            // Calculate the rotated center and verts
            glm::vec2 rotatedCenter;
            rotatedCenter.x = relativeCenter.x * cosAngle - relativeCenter.y * sinAngle;
            rotatedCenter.y = relativeCenter.x * sinAngle + relativeCenter.y * cosAngle;
            std::array<glm::vec2, 4> rotatedVerts;
            for (int i = 0; i < relativeVerts.size(); i++) {
                glm::vec2 vert = relativeVerts.at(i);
                glm::vec2 rotatedVert;
                rotatedVert.x = vert.x * cosAngle - vert.y * sinAngle;
                rotatedVert.y = vert.x * sinAngle + vert.y * cosAngle;
                rotatedVerts.at(i) = rotatedVert;
            }

            // Calculate the upper and lower x and y limits
            glm::vec2 maxes = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };
            glm::vec2 mins = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
            for (auto vert : rotatedVerts) {
                mins.x = std::min(mins.x, vert.x);
                mins.y = std::min(mins.y, vert.y);
                maxes.x = std::max(maxes.x, vert.x);
                maxes.y = std::max(maxes.y, vert.y);
            }

            // Determine if the center of the circle is withing the upper and lower x any y limits
            return rotatedCenter.x <= maxes.x && rotatedCenter.x >= mins.x
                && rotatedCenter.y <= maxes.y && rotatedCenter.y >= mins.y;

        }

    }

}