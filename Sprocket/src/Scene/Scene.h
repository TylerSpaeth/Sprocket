#ifndef SCENE_H
#define SCENE_H

#include "Core/Macros.h"

#include "Events/Event.h"

#include "Entity.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

  class SPROCKET_API Scene {

    friend class SceneManager;

    private:

      std::function<void(Event&)> m_EventCallback;

      std::vector<Entity*> m_Entities;

      /// @brief Registers the given function as an Event callback to be run when an event occurs. 
      /// The event handler should subscribe to this in order for events 
      /// produced here to become part of the central event system.
      /// @param eventCallback a function that will take in an Event when an event occurs.
      void RegisterEventCallback(const std::function<void(Event&)> eventCallback);

      /// @brief This is called when the scene becomes the active scene.
      void OnActivate();
      /// @brief This is called when the scene is no long the active scene.
      void OnDeactivate();
      /// @brief This corresponds to application updates.
      /// @param deltaTime The time since the last update
      void OnUpdate(float deltaTime);
      

    public:

      Scene();
      ~Scene();

      bool SubmitEntityToScene(Entity& entity);
      bool RemoveEntityFromScene(Entity& entity);

      bool AssignEntityParent(Entity& child, Entity* parent);

      /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
      /// instead be called directly be the Scene Manager when it recieves an event.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);

  };

}

#endif