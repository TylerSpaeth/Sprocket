#ifndef PHYSICS_H
#define PHYSICS_H

#include "Core/Macros.h"

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

#include "Core/Sprocket.pch"

namespace Sprocket {

    class PhysicsObject {
        friend class Physics;
    private:
        glm::vec2 m_ColliderCenter;
        std::vector<std::pair<int, int>> m_Regions; // The regions this object is in
        std::optional<float> m_BoxColliderRotation;
        std::optional<glm::vec2> m_BoxColliderSize;
        std::optional<float> m_CircleRadius;
        std::vector<unsigned int> m_CollidesWith;
        bool m_Valid = false;
        PhysicsObject(glm::vec2 colliderCenter, float boxColliderRotation, glm::vec2 boxColliderSize) : m_ColliderCenter(colliderCenter), m_BoxColliderRotation(boxColliderRotation), m_BoxColliderSize(boxColliderSize) {}
        PhysicsObject(glm::vec2 colliderCenter, float circleRadius) : m_ColliderCenter(colliderCenter), m_CircleRadius(circleRadius) {}
    };

    class Physics {

    private:

        static const float s_PixelsPerUnit;

        // Singleton Components
        static Physics* s_Instance;
        Physics() {}
        Physics(const Physics&) = delete;
        Physics operator=(const Physics&) = delete;

        static std::vector<PhysicsObject> m_PhysicsObjects;
        static std::vector<std::vector<unsigned int>> m_CollidesWith;
        static std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_FreeSlots;
        static std::map<std::pair<int, int>, std::vector<unsigned int>> m_Regions;

        // These values represent the maximum amount above or below that the average collider width 
        // and height can be in relation to the current box x and y sizes. A maximum decrease of .75 
        // means that everything down to and including .75 * current box size is acceptable without 
        // rehashing. A maximum increase of 1.25 means that everything up to and including 1.25 * 
        // current box size is acceptable without rehashing, 
        static float m_MaximumDecrease;
        static float m_MaximumIncrease;
        static float m_BoxXSize;
        static float m_BoxYSize;
        // BROAD PHASE
        /// @brief A helper function that adds the object to the region sets adds the region for 
        /// reverse lookup.
        /// @param coordinates The grid coordinate that should be associated with the object
        /// @param physicsID The id of the object to use.
        static void SetRegion(std::pair<int, int> coordinates, const unsigned int physicsID);
        /// @brief Calculates what regions this object belongs in and places it in all of them.
        /// @param physicsID The id of the object to place.
        static void PlaceInRegions(const unsigned int physicsID);
        /// @brief Removes the object from all regions.
        /// @param physicsID The id of the object to be removed.
        static void RemoveFromRegions(const unsigned int physicsID);
        /// @brief Compares the average collider size to the boxXSize and boxYSize. If the average is
        /// above of below the current size be more that the maximum decrease or increase percentage, 
        /// the the box sizes are reset to the averages and RehashAllObjects() is called.
        static void ValidateCurrentBoxSize();
        /// @brief Clears all objects out of all regions and then replaces all of the objects into 
        /// regions.
        static void RehashAllObjects();
        // NARROW PHASE
        /// @brief Clears out the m_CollidesWith vector. This needs to be called before 
        /// ProcessCollisions(), otherwise residual collision data from the previous frame will
        /// corrupt the data of the current frame.
        static void ClearPreviousCollisions();
        /// @brief Iterates over all of the regions in m_Regions and performs narrow phase collision 
        /// detection on all of the object contained in each region, updating m_CollidesWith.
        static void ProcessCollisions();
        /// @brief A helper function to update the m_CollidesWith vector to reflect a collision 
        /// between the two given physics objects. This function provides no input validation,
        /// so it is up to the caller to ensure that if this function is called, the two objects
        /// have actually collided.
        /// @param physicsID1 The id of the first object.
        /// @param physicsID2 The id of the second object.
        static void UpdateCollidesWith(const unsigned int physicsID1, const unsigned int physicsID2);
        /// @brief Checks if the two PhysicsObjects collide.
  /// @param physicsID1 the id of the first PhysicsObject
  /// @param physicsID2 the id of the second PhysicsObject
  /// @return true if they collide, false otherwise
        static bool CheckCollides(const unsigned int physicsID1, const unsigned int physicsID2);
        /// @brief Checks to see if this PhysicsObject collides with anything.
        /// @param physicsID the id of a PhysicsObject
        /// @return a populated vector if this PhysicsObject collides with anything, false otherwise
        static std::vector<unsigned int> CheckCollidesGeneric(const unsigned int physicsID);

        static unsigned int AddPhysicsObject(const glm::vec2 colliderCenter, const float boxColliderRotation, const glm::vec2 boxColliderSize);
        static unsigned int AddPhysicsObject(const glm::vec2 colliderCenter, const float circleRadius);
        static void UpdatePhysicsObject(const unsigned int physicsID, const glm::vec2 colliderCenter, const float boxColliderRotation, const glm::vec2 boxColliderSize);
        static void UpdatePhysicsObject(const unsigned int physicsID, const glm::vec2 colliderCenter, const float circleRadius);
        static void RemovePhysicsObject(const unsigned int physicsID);

    public:

        /// @brief Initializes the singleton. This must be called before any other Physics functions.
        static void Init();

        /// @brief Handles incoming events. Should be registered as a callback to recieve events.
        /// @param event The event the should be handled.
        static void OnEvent(Event& event);

    };

}

#endif 