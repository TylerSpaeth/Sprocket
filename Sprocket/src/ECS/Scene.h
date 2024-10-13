#ifndef SCENE_H
#define SCENE_H

#include "Core/Macros.h"
#include "ECS/Entity.h"
#include "Events/Event.h"

#include <functional>

namespace Sprocket {

  /// @brief This is the foundation for Sprocket applications. An application is a collection of 
  /// scenes that are made up of entities and components that define application behaviors.
  class SPROCKET_API Scene {
    friend class SceneManager;
    private:
      RootEntity* const m_Root = new RootEntity();
      std::function<void(Event&)> m_EventCallback;
    public:
      RootEntity* const GetSceneRoot() const {return m_Root;}

      ~Scene() {
        delete m_Root;
      }

      /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
      /// instead be called directly be the Scene Manager when it recieves an event.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);
  };


}

#endif