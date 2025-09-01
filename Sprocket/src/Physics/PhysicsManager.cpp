#include "PhysicsManager.h"

#include "Collision.h"

#include "Events/PhysicsEvent.h"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"

namespace Sprocket {

    // Static variable definitions
    const float PhysicsManager::s_PixelsPerUnit = 100;

    void PhysicsManager::OnEvent(Event& event) {
        EventType type = event.GetEventType();
        switch (type) {
        case EventType::APP_UPDATE: {
            OnUpdate();
            break;
        }
        case EventType::PHYSICS_NEW: {
            PhysicsNewEvent& newEvent = ((PhysicsNewEvent&)event);
            switch (newEvent.m_ColliderType) {
            case ColliderType::BOX:
                newEvent.m_PhysicsID = AddPhysicsObject(newEvent.m_ColliderCenter * s_PixelsPerUnit, newEvent.m_BoxColliderRotation, newEvent.m_BoxColliderSize * s_PixelsPerUnit);
                break;
            case ColliderType::CIRCLE:
                newEvent.m_PhysicsID = AddPhysicsObject(newEvent.m_ColliderCenter, newEvent.m_CircleRadius * s_PixelsPerUnit);
                break;
            }
            break;
        }
        case EventType::PHYSICS_UPDATE: {
            PhysicsUpdateEvent& updateEvent = ((PhysicsUpdateEvent&)event);
            switch (updateEvent.m_ColliderType) {
            case ColliderType::BOX:
                UpdatePhysicsObject(updateEvent.m_PhysicsID, updateEvent.m_ColliderCenter * s_PixelsPerUnit, updateEvent.m_BoxColliderRotation, updateEvent.m_BoxColliderSize * s_PixelsPerUnit);
                break;
            case ColliderType::CIRCLE:
                UpdatePhysicsObject(updateEvent.m_PhysicsID, updateEvent.m_ColliderCenter, updateEvent.m_CircleRadius * s_PixelsPerUnit);
                break;
            }
            break;
        }
        case EventType::PHYSICS_DELETE: {
            RemovePhysicsObject(((PhysicsDeleteEvent&)event).m_PhysicsID);
            break;
        }
        case EventType::PHYSICS_COLLISION_CHECK: {
            PhysicsCollisionCheckEvent& collisionCheckEvent = (PhysicsCollisionCheckEvent&)event;
            collisionCheckEvent.m_Collides = CheckCollides(collisionCheckEvent.m_PhysicsID1, collisionCheckEvent.m_PhysicsID2);
            break;
        }
        case EventType::PHYSICS_COLLISION_CHECK_GENERIC: {
            PhysicsCollisionCheckGenericEvent& collisionCheckGenericEvent = (PhysicsCollisionCheckGenericEvent&)event;
            collisionCheckGenericEvent.m_CollidesWith = CheckCollidesGeneric(collisionCheckGenericEvent.m_PhysicsID);
            collisionCheckGenericEvent.m_Collides = collisionCheckGenericEvent.m_CollidesWith.size() != 0;
            break;
        }
        case EventType::APP_START: {
            OnStart();
            break;
        }
        case EventType::APP_SHUTDOWN: {
            OnShutdown();
            break;
        }
        }

    }

    unsigned int PhysicsManager::AddPhysicsObject(const glm::vec2 colliderCenter, const float boxColliderRotation, glm::vec2 boxColliderSize) {

        PhysicsObject object(colliderCenter, boxColliderRotation, boxColliderSize);
        object.m_Valid = true;

        unsigned int freeSlot = m_FreeSlots.top();

        // If there is only 1 entry in the queue then it must be at the end of the vector, so the next 
        // free slot needs to be added
        if (m_FreeSlots.size() == 1) {
            m_FreeSlots.push(freeSlot + 1);
        }

        m_FreeSlots.pop();

        // Put the object in the correct slot
        if (freeSlot == m_PhysicsObjects.size()) {
            m_PhysicsObjects.push_back(object);
            m_CollidesWith.push_back(std::vector<unsigned int>());
        }
        else {
            m_PhysicsObjects.at(freeSlot) = object;
            m_CollidesWith.at(freeSlot).clear();
        }

        PlaceInRegions(freeSlot);

        // This is the index where the object is stored
        return freeSlot;
    }

    unsigned int PhysicsManager::AddPhysicsObject(const glm::vec2 colliderCenter, const float circleRadius) {

        PhysicsObject object(colliderCenter, circleRadius);
        object.m_Valid = true;

        unsigned int freeSlot = m_FreeSlots.top();

        // If there is only 1 entry in the queue then it must be at the end of the vector, so the next 
        // free slot needs to be added
        if (m_FreeSlots.size() == 1) {
            m_FreeSlots.push(freeSlot + 1);
        }

        m_FreeSlots.pop();

        // Put the object in the correct slot
        if (freeSlot == m_PhysicsObjects.size()) {
            m_PhysicsObjects.push_back(object);
            m_CollidesWith.push_back(std::vector<unsigned int>());
        }
        else {
            m_PhysicsObjects.at(freeSlot) = object;
            m_CollidesWith.at(freeSlot).clear();
        }

        PlaceInRegions(freeSlot);

        // This is the index where the object is stored
        return freeSlot;
    }

    void PhysicsManager::UpdatePhysicsObject(const unsigned int physicsID, const glm::vec2 colliderCenter, const float boxColliderRotation, const glm::vec2 boxColliderSize) {
        m_PhysicsObjects.at(physicsID).m_ColliderCenter = colliderCenter;
        m_PhysicsObjects.at(physicsID).m_BoxColliderRotation = boxColliderRotation;
        m_PhysicsObjects.at(physicsID).m_BoxColliderSize = boxColliderSize;
        RemoveFromRegions(physicsID);
        PlaceInRegions(physicsID);
    }

    void PhysicsManager::UpdatePhysicsObject(const unsigned int physicsID, const glm::vec2 colliderCenter, const float circleRadius) {
        m_PhysicsObjects.at(physicsID).m_ColliderCenter = colliderCenter;
        m_PhysicsObjects.at(physicsID).m_CircleRadius = circleRadius;
        RemoveFromRegions(physicsID);
        PlaceInRegions(physicsID);
    }

    void PhysicsManager::RemovePhysicsObject(const unsigned int physicsID) {
        auto physicsObject = m_PhysicsObjects.at(physicsID);
        physicsObject.m_Valid = false;
        m_FreeSlots.push(physicsID);
        RemoveFromRegions(physicsID);
    }

    bool PhysicsManager::CheckCollides(const unsigned int physicsID1, const unsigned int physicsID2) {
        auto collidesWith = m_CollidesWith.at(physicsID1);
        if (std::find(collidesWith.begin(), collidesWith.end(), physicsID2) != collidesWith.end()) {
            return true;
        }
        return false;
    }

    std::vector<unsigned int> PhysicsManager::CheckCollidesGeneric(const unsigned int physicsID) {
        return m_CollidesWith.at(physicsID);
    }

    std::array<glm::vec2, 4> GetVerts(const glm::vec2 colliderCenter, const float boxColliderRotation, const glm::vec2 boxColliderSize) {
        glm::mat4 translation = glm::translate(glm::mat4(1), { colliderCenter,0 });
        glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(-boxColliderRotation), glm::vec3(0, 0, 1));

        glm::mat4 transform = translation * rotation;

        glm::vec2 v1 = transform * glm::vec4(-boxColliderSize.x / 2, boxColliderSize.y / 2, 0, 1);
        glm::vec2 v2 = transform * glm::vec4(-boxColliderSize.x / 2, -boxColliderSize.y / 2, 0, 1);
        glm::vec2 v3 = transform * glm::vec4(boxColliderSize.x / 2, -boxColliderSize.y / 2, 0, 1);
        glm::vec2 v4 = transform * glm::vec4(boxColliderSize.x / 2, boxColliderSize.y / 2, 0, 1);

        return { v1,v2,v3,v4 };
    }

    void PhysicsManager::SetRegion(std::pair<int, int> coordinates, const unsigned int physicsID) {

        if (!m_Regions.count(coordinates)) {
            m_Regions.insert({ coordinates, std::vector<unsigned int>() });
        }
        m_Regions.at(coordinates).push_back(physicsID);
        m_PhysicsObjects.at(physicsID).m_Regions.push_back(coordinates);
    }

    void PhysicsManager::PlaceInRegions(const unsigned int physicsID) {

        auto physicsObject = m_PhysicsObjects.at(physicsID);
        bool boxCollider = physicsObject.m_BoxColliderSize.has_value();

        std::optional<std::array<glm::vec2, 4>> boxColliderVerts;
        if (boxCollider) {
            boxColliderVerts = GetVerts(physicsObject.m_ColliderCenter, physicsObject.m_BoxColliderRotation.value(), physicsObject.m_BoxColliderSize.value());
        }

        int startXRegion = physicsObject.m_ColliderCenter.x / m_BoxXSize;
        int startYRegion = physicsObject.m_ColliderCenter.y / m_BoxYSize;

        // The object must collider with the region where its center is so there is no need to check
        // for the collision.
        SetRegion({ startXRegion,startYRegion }, physicsID);

        // The number of grid regions in width and height to be checked on the current pass
        int searchGridSize = 3;

        bool collisionDetected = true;

        // Continue looping so long as a there was a collision found in the previous iteration
        while (collisionDetected) {

            collisionDetected = false;
            int tmp;

            // Run along the right edge
            int xRegion = startXRegion + (searchGridSize - 1) / 2;
            int yRegion = startYRegion + (searchGridSize - 1) / 2;
            tmp = yRegion + 1 - searchGridSize;
            while (yRegion > tmp) {

                auto regionVerts = GetVerts({ xRegion * m_BoxXSize,yRegion * m_BoxYSize }, 0, { m_BoxXSize,m_BoxYSize });

                if (boxCollider) {
                    if (Collision::Collides(regionVerts, boxColliderVerts.value())) {
                        SetRegion({ xRegion,yRegion }, physicsID);
                        collisionDetected = true;
                    }
                }
                else {
                    if (Collision::Collides(regionVerts, { xRegion * m_BoxXSize, yRegion * m_BoxYSize }, 0, physicsObject.m_ColliderCenter, physicsObject.m_CircleRadius.value())) {
                        SetRegion({ xRegion,yRegion }, physicsID);
                        collisionDetected = true;
                    }
                }

                yRegion--;
            }

            // Run along the bottom
            tmp = xRegion + 1 - searchGridSize;
            while (xRegion > tmp) {

                auto regionVerts = GetVerts({ xRegion * m_BoxXSize,yRegion * m_BoxYSize }, 0, { m_BoxXSize,m_BoxYSize });

                if (boxCollider) {
                    if (Collision::Collides(regionVerts, boxColliderVerts.value())) {
                        SetRegion({ xRegion,yRegion }, physicsID);
                        collisionDetected = true;
                    }
                }
                else {
                    if (Collision::Collides(regionVerts, { xRegion * m_BoxXSize, yRegion * m_BoxYSize }, 0, physicsObject.m_ColliderCenter, physicsObject.m_CircleRadius.value())) {
                        SetRegion({ xRegion,yRegion }, physicsID);
                        collisionDetected = true;
                    }
                }

                xRegion--;
            }

            // Run along left edge
            tmp = yRegion - 1 + searchGridSize;
            while (yRegion < tmp) {

                auto regionVerts = GetVerts({ xRegion * m_BoxXSize,yRegion * m_BoxYSize }, 0, { m_BoxXSize,m_BoxYSize });

                if (boxCollider) {
                    if (Collision::Collides(regionVerts, boxColliderVerts.value())) {
                        SetRegion({ xRegion,yRegion }, physicsID);
                        collisionDetected = true;
                    }
                }
                else {
                    if (Collision::Collides(regionVerts, { xRegion * m_BoxXSize, yRegion * m_BoxYSize }, 0, physicsObject.m_ColliderCenter, physicsObject.m_CircleRadius.value())) {
                        SetRegion({ xRegion,yRegion }, physicsID);
                        collisionDetected = true;
                    }
                }

                yRegion++;
            }

            // Run along the top edge
            tmp = xRegion - 1 + searchGridSize;
            while (xRegion < tmp) {

                auto regionVerts = GetVerts({ xRegion * m_BoxXSize,yRegion * m_BoxYSize }, 0, { m_BoxXSize,m_BoxYSize });

                if (boxCollider) {
                    if (Collision::Collides(regionVerts, boxColliderVerts.value())) {
                        SetRegion({ xRegion,yRegion }, physicsID);
                        collisionDetected = true;
                    }
                }
                else {
                    if (Collision::Collides(regionVerts, { xRegion * m_BoxXSize, yRegion * m_BoxYSize }, 0, physicsObject.m_ColliderCenter, physicsObject.m_CircleRadius.value())) {
                        SetRegion({ xRegion,yRegion }, physicsID);
                        collisionDetected = true;
                    }
                }

                xRegion++;
            }

            // Expand the grid to check 1 more grid cell on each size
            searchGridSize += 2;

        }

    }

    void PhysicsManager::RemoveFromRegions(const unsigned int physicsID) {
        auto& physicsObject = m_PhysicsObjects.at(physicsID);
        auto regionsCopy = physicsObject.m_Regions; // Make a copy

        for (const auto& region : regionsCopy) {
            auto& regionVec = m_Regions.at(region);
            auto it = std::find(regionVec.begin(), regionVec.end(), physicsID);
            if (it != regionVec.end())
                regionVec.erase(it);
        }
        physicsObject.m_Regions.clear(); // Clear all regions from the object
    }

    void PhysicsManager::ValidateCurrentBoxSize() {

        float averageColliderWidth = 0;
        float averageColliderHeight = 0;
        unsigned int colliderCount = 0;

        for (int i = 0; i < m_PhysicsObjects.size(); i++) {
            auto physicsObject = m_PhysicsObjects.at(i);

            if (physicsObject.m_BoxColliderSize.has_value()) {
                averageColliderWidth += physicsObject.m_BoxColliderSize.value().x;
                averageColliderHeight += physicsObject.m_BoxColliderSize.value().y;
                colliderCount++;
            }
            else if (physicsObject.m_CircleRadius.has_value()) {
                averageColliderWidth += physicsObject.m_CircleRadius.value() * 2;
                averageColliderHeight += physicsObject.m_CircleRadius.value() * 2;
                colliderCount++;
            }
        }

        if (colliderCount == 0) return;

        averageColliderWidth /= colliderCount;
        averageColliderHeight /= colliderCount;

        float upperXLimit = m_BoxXSize * m_MaximumIncrease;
        float lowerXLimit = m_BoxXSize * m_MaximumDecrease;
        float upperYLimit = m_BoxYSize * m_MaximumIncrease;
        float lowerYLimit = m_BoxYSize * m_MaximumDecrease;

        if (averageColliderWidth < lowerXLimit || averageColliderWidth > upperXLimit
            || averageColliderHeight < lowerYLimit || averageColliderHeight > upperYLimit) {
            m_BoxXSize = averageColliderWidth;
            m_BoxYSize = averageColliderHeight;
            RehashAllObjects();
        }

    }

    void PhysicsManager::RehashAllObjects() {
        for (int i = 0; i < m_PhysicsObjects.size(); i++) {
            if (m_PhysicsObjects.at(i).m_Valid) {
                RemoveFromRegions(i);
                PlaceInRegions(i);
            }
        }
    }

    void PhysicsManager::ClearPreviousCollisions() {
        for (int i = 0; i < m_CollidesWith.size(); i++) {
            m_CollidesWith.at(i).clear();
        }
    }

    void PhysicsManager::ProcessCollisions() {

        for (const auto& region : m_Regions) {

            auto physicsObjects = region.second;

            for (auto physicsObjectID1 : physicsObjects) {
                for (auto physicsObjectID2 = std::find(physicsObjects.begin(), physicsObjects.end(), physicsObjectID1); physicsObjectID2 != physicsObjects.end(); physicsObjectID2++) {
                    if (physicsObjectID1 == *physicsObjectID2) continue;

                    auto physicsObject1 = m_PhysicsObjects.at(physicsObjectID1);
                    auto physicsObject2 = m_PhysicsObjects.at(*physicsObjectID2);

                    // If true, obj1 has a box collider, if false then circle collider
                    bool boxCollider1 = physicsObject1.m_BoxColliderSize.has_value();
                    // If true, obj2 has a box collider, if false then circle collider
                    bool boxCollider2 = physicsObject2.m_BoxColliderSize.has_value();

                    std::optional<std::array<glm::vec2, 4>> boxVerts1;
                    std::optional<std::array<glm::vec2, 4>> boxVerts2;

                    if (boxCollider1) {
                        boxVerts1 = GetVerts(physicsObject1.m_ColliderCenter, physicsObject1.m_BoxColliderRotation.value(), physicsObject1.m_BoxColliderSize.value());
                    }
                    if (boxCollider2) {
                        boxVerts2 = GetVerts(physicsObject2.m_ColliderCenter, physicsObject2.m_BoxColliderRotation.value(), physicsObject2.m_BoxColliderSize.value());
                    }

                    // Check box-box collision
                    if (boxCollider1 && boxCollider2) {
                        // If there is a collision between the boxes
                        if (Collision::Collides(boxVerts1.value(), boxVerts2.value())) {
                            UpdateCollidesWith(physicsObjectID1, *physicsObjectID2);
                        }
                    }

                    // Check box-circle collision
                    else if (boxCollider1 && !boxCollider2) {

                        // If there is a collision between the shapes
                        if (Collision::Collides(boxVerts1.value(), physicsObject1.m_ColliderCenter, physicsObject1.m_BoxColliderRotation.value(), physicsObject2.m_ColliderCenter, physicsObject2.m_CircleRadius.value())) {
                            UpdateCollidesWith(physicsObjectID1, *physicsObjectID2);
                        }

                    }

                    // Check circle-box collision
                    else if (boxCollider2 && !boxCollider1) {

                        // If there is a collision between the shapes
                        if (Collision::Collides(boxVerts2.value(), physicsObject2.m_ColliderCenter, physicsObject2.m_BoxColliderRotation.value(), physicsObject1.m_ColliderCenter, physicsObject1.m_CircleRadius.value())) {
                            UpdateCollidesWith(physicsObjectID1, *physicsObjectID2);
                        }

                    }

                    // Check circle-circle collision
                    else {

                        // If there is a collision between the circles
                        if (Collision::Collides(physicsObject1.m_ColliderCenter, physicsObject1.m_CircleRadius.value(), physicsObject2.m_ColliderCenter, physicsObject2.m_CircleRadius.value())) {
                            UpdateCollidesWith(physicsObjectID1, *physicsObjectID2);
                        }

                    }

                }

            }

        }

    }

    void PhysicsManager::UpdateCollidesWith(const unsigned int physicsID1, const unsigned int physicsID2) {
        m_CollidesWith.at(physicsID1).push_back(physicsID2);
        m_CollidesWith.at(physicsID2).push_back(physicsID1);
    }

    void PhysicsManager::OnStart() {
        m_FreeSlots.push(0);
        m_MaximumDecrease = .75;
        m_MaximumIncrease = 1.25;
        m_BoxXSize = std::numeric_limits<float>::max();
        m_BoxYSize = std::numeric_limits<float>::max();
    }

    void PhysicsManager::OnUpdate() {
        ValidateCurrentBoxSize();
        ClearPreviousCollisions();
        ProcessCollisions();
    }

    void PhysicsManager::OnShutdown() {
        delete this;
    }

}