#ifndef SCENE_H
#define SCENE_H

#include "Core/Macros.h"
#include "ECS/Entity.h"
#include "Events/Event.h"

#include "vector"

namespace Sprocket {

  class SPROCKET_API Scene {
    public:
      /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
      /// instead be called directly be the Scene Manager when it recieves an event.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);
  };


}

#endif