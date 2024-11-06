#ifndef SCENE_H
#define SCENE_H

#include "Core/Macros.h"
#include "Events/Event.h"
#include "ECS/Component.h"

#include <map>
#include <vector>
#include <queue>
#include <stdexcept>

namespace Sprocket {

  /// @brief This is the foundation for Sprocket applications. An application is a collection of 
  /// scenes that are made up components, grouped into entities, that define application behaviors.
  class SPROCKET_API Scene {

    private:

      // FIXME change these to vectors to utilize contiguous memory blocks

      // SCENE TREE COMPONENTS
      unsigned int m_EntityCount = 0;
      // Stores the parentID of a given entity, if the parentID is -1, that indicates no parent
      std::vector<int> m_Parents;
      std::vector<std::vector<unsigned int>> m_Children;
      // A queue holding the index of deleted entities 
      std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_DeletedEntities;

      // POSITION COMPONENTS
      // These can not be added or removed, only updated. To remove a transform would be to delete
      // the entity.

      // Local space transforms
      std::vector<TransformComponent> m_Transforms;
      // Global space transforms. Note that the global transform for a given entity actually 
      // corresponds to the global position for its parent. To get the true global position, the 
      // local transform needs to be applied.
      std::vector<TransformComponent> m_GlobalTransforms;

      // RENDERING COMPONENTS
      std::map<unsigned int, QuadRendererComponent> m_QuadRenderers;
      unsigned int m_CameraEntityID = -1;

      // PHYSICS COMPONENTS
      void* m_Physics; // A pointer to a Physics object
      std::map<unsigned int, BoxColliderComponent> m_BoxColliders;
      std::map<unsigned int, CircleColliderComponent> m_CircleColliders;
      std::map<unsigned int, PhysicsComponent> m_PhysicsComponents;

      void OnUpdate(float deltaTime);
      void OnClose();

      // HELPER FUNCTIONS
      void RemoveQuadRenderer(const unsigned int entityID);
      void RemovePhysicsObjectCollider(const unsigned int entityID);
      void RemovePhysicsObject(const unsigned int entityID);
      
    public:

      Scene();
      ~Scene();

      /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
      /// instead be called directly be the Scene Manager when it recieves an event.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);

      // Creates a transform and sets an id for a new entity
      unsigned int CreateEntity();
      // Functionality of functions is undefined if entityID is used after deletion
      void DeleteEntity(const unsigned int entityID); 
      void SetEntityParent(const unsigned int entityID, const unsigned int parentID);

      // TODO make sure that these functions make appropriate calls to the physics system.

      // TODO overload for all component types that can be added
      void AddComponent(const unsigned int entityID, const QuadRendererComponent& component);
      void AddComponent(const unsigned int entityID, const CameraComponent& component);
      void AddComponent(const unsigned int entityID, const BoxColliderComponent& component);
      void AddComponent(const unsigned int entityID, const CircleColliderComponent& component);
      void AddComponent(const unsigned int entityID, const PhysicsComponent& component);
      
      // TODO overload for all component types that can be added
      void UpdateComponent(const unsigned int entityID, const TransformComponent& replacement);
      void UpdateComponent(const unsigned int entityID, const QuadRendererComponent& replacement);
      void UpdateComponent(const unsigned int entityID, const BoxColliderComponent& replacement);
      void UpdateComponent(const unsigned int entityID, const CircleColliderComponent& replacement);
      void UpdateComponent(const unsigned int entityID, const PhysicsComponent& component);

      template<typename T>
      T GetComponent(const unsigned int entityID) {
        throw std::invalid_argument("You can only get components of a valid component type.");
      }

      template<typename T>
      void RemoveComponent(const unsigned int entityID) {
        throw std::invalid_argument("You can only remove components of a valid component type.");
      }

      bool CheckCollides(const unsigned int entityID) const;
      bool CheckCollides(const unsigned int entityID, const unsigned int otherEntityID) const;

  };


  // TODO Add template for all component types

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////// RETRIEVAL ///////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  
  template<>
  inline TransformComponent Scene::GetComponent<TransformComponent>(const unsigned int entityID) {
    return m_Transforms.at(entityID);
  }

  template<>
  inline QuadRendererComponent Scene::GetComponent<QuadRendererComponent>(const unsigned int entityID) {
    if(!m_QuadRenderers.count(entityID)) {
      throw std::invalid_argument("There is no QuadRendererComponent for this entity.");
    }
    return m_QuadRenderers.at(entityID);
  }

  template<>
  inline BoxColliderComponent Scene::GetComponent<BoxColliderComponent>(const unsigned int entityID) {
    if(!m_BoxColliders.count(entityID)) {
      throw std::invalid_argument("There is no BoxColliderComponent for this entity.");
    }
    return m_BoxColliders.at(entityID);
  }

  template<>
  inline CircleColliderComponent Scene::GetComponent<CircleColliderComponent>(const unsigned int entityID) {
    if(!m_CircleColliders.count(entityID)) {
      throw std::invalid_argument("There is no CircleColliderComponent for this entity.");
    }
    return m_CircleColliders.at(entityID);
  }

  template<>
  inline PhysicsComponent Scene::GetComponent<PhysicsComponent>(const unsigned int entityID) {
    if(!m_PhysicsComponents.count(entityID)) {
      throw std::invalid_argument("There is not PhysicsComponent for this entity.");
    }
    return m_PhysicsComponents.at(entityID);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////// REMOVAL ////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  template<>
  inline void Scene::RemoveComponent<QuadRendererComponent>(const unsigned int entityID) {
    if(m_QuadRenderers.count(entityID)) {
      RemoveQuadRenderer(entityID);
      return;
    }
    
    throw std::invalid_argument("This entity does not have a QuadRendererComponent.");
  }

  template<>
  inline void Scene::RemoveComponent<BoxColliderComponent>(const unsigned int entityID) {
    if(m_BoxColliders.count(entityID)) {
      m_BoxColliders.extract(entityID);

      // Since any entity with a collider must have physics component, remove the collider
      // from the corresponding physics object
      RemovePhysicsObjectCollider(entityID);

      return;
    }

    throw std::invalid_argument("This entity does not have a BoxColliderComponent.");
  }

  template<>
  inline void Scene::RemoveComponent<CircleColliderComponent>(const unsigned int entityID) {
    if(m_CircleColliders.count(entityID)) {
      m_CircleColliders.extract(entityID);
      
      // Since any entity with a collider must have physics component, remove the collider
      // from the corresponding physics object
      RemovePhysicsObjectCollider(entityID);

      return;
    }
    
    throw std::invalid_argument("This entity does not have a CircleColliderComponent.");
  }

  template<>
  inline void Scene::RemoveComponent<ColliderComponent>(const unsigned int entityID) {
    // First try to remove BoxColliderComponent, if that fails try to remove a 
    // CircleColliderComponent. If that too fails, then there are no colliders on this entity.
    try {
      RemoveComponent<BoxColliderComponent>(entityID);
    }
    catch(const std::exception& e) {
      try {
        RemoveComponent<CircleColliderComponent>(entityID);
      }
      catch(const std::exception& e) {
        throw std::invalid_argument("This entity does not have a collider.");
      }
    }
  }

  template<>
  inline void Scene::RemoveComponent<CameraComponent>(const unsigned int entityID) {
    if(entityID != m_CameraEntityID) {
      throw std::invalid_argument("This entity does not have a camera component.");
    }
    m_CameraEntityID = -1;
  }

  template<>
  inline void Scene::RemoveComponent<PhysicsComponent>(const unsigned int entityID) {
    if(m_PhysicsComponents.count(entityID)) {

      RemovePhysicsObject(entityID);

      m_PhysicsComponents.extract(entityID);
      return;
    }

    throw std::invalid_argument("This entity does not have a PhysicsComponent.");
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////


}

#endif