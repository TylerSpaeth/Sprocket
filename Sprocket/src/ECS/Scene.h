#ifndef SCENE_H
#define SCENE_H

#include "Core/Macros.h"
#include "Events/Event.h"
#include "ECS/Component.h"
#include "ECS/Collision.h"

#include <map>
#include <vector>
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

      // POSITION COMPONENTS
      // Local space transforms
      std::vector<TransformComponent> m_Transforms;
      // Global space transforms. Note that the global transform for a given entity actually 
      // corresponds to the global position for its parent. To get the true global position, the 
      // local transform needs to be applied.
      std::vector<TransformComponent> m_GlobalTransforms;

      // RENDERING COMPONENTS
      std::map<unsigned int, QuadRendererComponent> m_QuadRenderers;
      unsigned int m_CameraEntityID = -1;

      // TODO only allow an entity to have one or another
      // PHYSICS COMPONENTS
      std::map<unsigned int, BoxColliderComponent> m_BoxColliders;
      std::map<unsigned int, CircleColliderComponent> m_CircleColliders;
      
    public:

      /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
      /// instead be called directly be the Scene Manager when it recieves an event.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);

      // Creates a transform and sets an id for a new entity
      unsigned int CreateEntity(); 
      void SetEntityParent(const unsigned int entityID, const unsigned int parentID);

      // TODO overload for all component types that can be added
      void AddComponent(const unsigned int entityID, const QuadRendererComponent& component);
      void AddComponent(const unsigned int entityID, const CameraComponent& component);
      void AddComponent(const unsigned int entityID, const BoxColliderComponent& component);
      void AddComponent(const unsigned int entityID, const CircleColliderComponent& component);
      
      // TODO overload for all component types that can be added
      void UpdateComponent(const unsigned int entityID, const TransformComponent& replacement);
      void UpdateComponent(const unsigned int entityID, const QuadRendererComponent& replacement);
      void UpdateComponent(const unsigned int entityID, const BoxColliderComponent& replacement);
      void UpdateComponent(const unsigned int entityID, const CircleColliderComponent& replacement);

      template<typename T>
      T GetComponent(const unsigned int entityID) {
        std::invalid_argument("You can only get components of a valid component type.");
      }

      bool CheckCollides(const unsigned int entityID) const;
      bool CheckCollides(const unsigned int entityID, const unsigned int otherEntityID) const;

  };


  // TODO Add template for all component types
  
  template<>
  inline TransformComponent Scene::GetComponent<TransformComponent>(const unsigned int entityID) {
    return m_Transforms.at(entityID);
  }

  template<>
  inline QuadRendererComponent Scene::GetComponent<QuadRendererComponent>(const unsigned int entityID) {
    return m_QuadRenderers.at(entityID);
  }

  template<>
  inline BoxColliderComponent Scene::GetComponent<BoxColliderComponent>(const unsigned int entityID) {
    return m_BoxColliders.at(entityID);
  }

  template<>
  inline CircleColliderComponent Scene::GetComponent<CircleColliderComponent>(const unsigned int entityID) {
    return m_CircleColliders.at(entityID);
  }

}

#endif