#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

#include "Core/Sprocket.pch"

namespace Sprocket {
        
    /// @brief A helper class that stores the necessary data for a single physics object.
    /// Depending on the object some of the values may not be used.
    class PhysicsObject {
        friend class PhysicsManager;
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
    
    /// @brief The handler for the physics system
    class PhysicsManager {

    private:

        std::vector<PhysicsObject> m_PhysicsObjects;
        std::vector<std::vector<unsigned int>> m_CollidesWith;
        std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_FreeSlots;
        std::map<std::pair<int, int>, std::vector<unsigned int>> m_Regions;

        // These values represent the maximum amount above or below that the average collider width 
        // and height can be in relation to the current box x and y sizes. A maximum decrease of .75 
        // means that everything down to and including .75 * current box size is acceptable without 
        // rehashing. A maximum increase of 1.25 means that everything up to and including 1.25 * 
        // current box size is acceptable without rehashing, 

        float m_MaximumDecrease;
        float m_MaximumIncrease;
        float m_BoxXSize;
        float m_BoxYSize;

    public:

        /// @brief Handles incoming events. Should be registered as a callback to recieve events.
        /// @param event The event the should be handled.
        void OnEvent(Event& event);

    private: 

        // BROAD PHASE
        /// @brief A helper function that adds the object to the region sets adds the region for 
        /// reverse lookup.
        /// @param coordinates The grid coordinate that should be associated with the object
        /// @param physicsID The id of the object to use.
        void SetRegion(std::pair<int, int> coordinates, const unsigned int physicsID);
        /// @brief Calculates what regions this object belongs in and places it in all of them.
        /// @param physicsID The id of the object to place.
        void PlaceInRegions(const unsigned int physicsID);
        /// @brief Removes the object from all regions.
        /// @param physicsID The id of the object to be removed.
        void RemoveFromRegions(const unsigned int physicsID);
        /// @brief Compares the average collider size to the boxXSize and boxYSize. If the average is
        /// above of below the current size be more that the maximum decrease or increase percentage, 
        /// the the box sizes are reset to the averages and RehashAllObjects() is called.
        void ValidateCurrentBoxSize();
        /// @brief Clears all objects out of all regions and then replaces all of the objects into 
        /// regions.
        void RehashAllObjects();
        // NARROW PHASE
        /// @brief Clears out the m_CollidesWith vector. This needs to be called before 
        /// ProcessCollisions(), otherwise residual collision data from the previous frame will
        /// corrupt the data of the current frame.
        void ClearPreviousCollisions();
        /// @brief Iterates over all of the regions in m_Regions and performs narrow phase collision 
        /// detection on all of the object contained in each region, updating m_CollidesWith.
        void ProcessCollisions();
        /// @brief A helper function to update the m_CollidesWith vector to reflect a collision 
        /// between the two given physics objects. This function provides no input validation,
        /// so it is up to the caller to ensure that if this function is called, the two objects
        /// have actually collided.
        /// @param physicsID1 The id of the first object.
        /// @param physicsID2 The id of the second object.
        void UpdateCollidesWith(const unsigned int physicsID1, const unsigned int physicsID2);
        /// @brief Checks if the two PhysicsObjects collide.
        /// @param physicsID1 the id of the first PhysicsObject
        /// @param physicsID2 the id of the second PhysicsObject
        /// @return true if they collide, false otherwise
        bool CheckCollides(const unsigned int physicsID1, const unsigned int physicsID2);
        /// @brief Checks to see if this PhysicsObject collides with anything.
        /// @param physicsID the id of a PhysicsObject
        /// @return a populated vector if this PhysicsObject collides with anything, false otherwise
        const std::vector<unsigned int> CheckCollidesGeneric(const unsigned int physicsID);

        /// @brief Adds a new box collider PhysicsObject into the system.
        /// @param colliderCenter - The center of the collider.
        /// @param boxColliderRotation - The z-rotation of the box collider in degrees.
        /// @param boxColliderSize - The size of the box collider.
        /// @returns The index of the new object, that can be used for future modification or removal.
        const unsigned int AddPhysicsObject(const glm::vec2 colliderCenter, const float boxColliderRotation, const glm::vec2 boxColliderSize);

        /// @brief Adds a new circle collider PhysicsObject into the system.
        /// @param colliderCenter - The center of the collider.
        /// @param circleRadius - The radius of the collider.
        /// @returns The index of the new object, that can be used for future modification or removal.
        const unsigned int AddPhysicsObject(const glm::vec2 colliderCenter, const float circleRadius);

        /// @brief Updates to box collider at the given physicsID.
        /// @param physicsID - The id to lookup the collider.
        /// @param colliderCenter - The center of the collider.
        /// @param boxColliderRotation - The z-rotation of the box collider in degrees.
        /// @param boxColliderSize - The size of the box collider.
        void UpdatePhysicsObject(const unsigned int physicsID, const glm::vec2 colliderCenter, const float boxColliderRotation, const glm::vec2 boxColliderSize);

        /// @brief Updates to circle collider at the given physicsID.
        /// @param physicsID - The id to lookup the collider.
        /// @param colliderCenter - The center of the collider.
        /// @param circleRadius - The radius of the collider.
        void UpdatePhysicsObject(const unsigned int physicsID, const glm::vec2 colliderCenter, const float circleRadius);

        /// @brief Removes the PhysicsObject from the system.
        /// @param physicsID - The id of the object to remove.
        void RemovePhysicsObject(const unsigned int physicsID);

        /// @brief Logic to occur on application startup.
        void OnStart();
        /// @brief Logic to occur on application update.
        void OnUpdate();
        /// @brief Logic to occur on application shutdown.
        void OnShutdown();

    };

}

#endif 