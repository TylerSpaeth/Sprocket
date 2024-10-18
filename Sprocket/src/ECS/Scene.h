#ifndef SCENE_H
#define SCENE_H

#include "Core/Macros.h"
#include "Events/Event.h"
#include "ECS/Component.h"

#include <map>
#include <vector>

namespace Sprocket {

  /// @brief This is the foundation for Sprocket applications. An application is a collection of 
  /// scenes that are made up of entities and components that define application behaviors.
  class SPROCKET_API Scene {

    private:
      
      unsigned int m_EntityCount = 0;
      std::map<unsigned int, std::vector<unsigned int>> m_Children;
      // FIXME change these to vectors to utilize contiguous memory blocks
      std::map<unsigned int, TransformComponent> m_Transforms;
      std::map<unsigned int, QuadRendererComponent> m_QuadRenderers;
      
    public:

      /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
      /// instead be called directly be the Scene Manager when it recieves an event.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);

      // Creates a transform and sets an id for a new entity
      unsigned int CreateEntity(); 
      void SetEntityParent(const unsigned int entityID, const unsigned int parentID);

      void AddComponent(const unsigned int entityID, const Component& component);
      void UpdateComponent(const unsigned int enitityID, const Component& replacement);
      
      TransformComponent GetTransform(const unsigned int entityID);
      QuadRendererComponent GetQuadRenderer(const unsigned int entityID);

      
  };

}

#endif