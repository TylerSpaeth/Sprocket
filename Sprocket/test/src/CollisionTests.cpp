#include <gtest/gtest.h>
#include "Physics/Collision.h"
#include <array>
#include <../src/ThirdParty/glm/glm.hpp>

using namespace Sprocket;

// Helper for box vertices (axis-aligned, centered at pos, size w x h)
std::array<glm::vec2, 4> MakeBox(glm::vec2 pos, float w, float h) {
    float hw = w / 2.0f, hh = h / 2.0f;
    return {
        glm::vec2{pos.x + hw, pos.y + hh},
        glm::vec2{pos.x + hw, pos.y - hh},
        glm::vec2{pos.x - hw, pos.y - hh},
        glm::vec2{pos.x - hw, pos.y + hh}
    };
}

TEST(CollisionTests, CircleCircleCollision) {
    glm::vec2 c1{0, 0};
    glm::vec2 c2{1, 0};
    float r1 = 1.0f, r2 = 1.0f;
    EXPECT_TRUE(Collision::Collides(c1, r1, c2, r2)); // Overlapping

    c2 = glm::vec2{3, 0};
    EXPECT_FALSE(Collision::Collides(c1, r1, c2, r2)); // Not overlapping

    c2 = glm::vec2{2, 0};
    EXPECT_TRUE(Collision::Collides(c1, r1, c2, r2)); // Touching
}

TEST(CollisionTests, BoxBoxCollision) {
    auto box1 = MakeBox({0, 0}, 2, 2);
    auto box2 = MakeBox({1, 0}, 2, 2);
    EXPECT_TRUE(Collision::Collides(box1, box2)); // Overlapping

    box2 = MakeBox({3, 0}, 2, 2);
    EXPECT_FALSE(Collision::Collides(box1, box2)); // Not overlapping

    box2 = MakeBox({2, 0}, 2, 2);
    EXPECT_TRUE(Collision::Collides(box1, box2)); // Touching at edge
}

TEST(CollisionTests, BoxCircleCollision) {
    auto box = MakeBox({0, 0}, 2, 2);
    glm::vec2 boxPos{0, 0};
    float boxRot = 0.0f;

    glm::vec2 circleCenter{0, 0};
    float circleRadius = 1.0f;
    EXPECT_TRUE(Collision::Collides(box, boxPos, boxRot, circleCenter, circleRadius)); // Inside

    circleCenter = glm::vec2{2.5f, 0};
    EXPECT_FALSE(Collision::Collides(box, boxPos, boxRot, circleCenter, circleRadius)); // Outside

    circleCenter = glm::vec2{1.0f, 0};
    EXPECT_TRUE(Collision::Collides(box, boxPos, boxRot, circleCenter, circleRadius)); // Touching edge

    // Rotated box
    boxRot = 45.0f;
    circleCenter = glm::vec2{0, 1.2f};
    EXPECT_TRUE(Collision::Collides(box, boxPos, boxRot, circleCenter, circleRadius));
}