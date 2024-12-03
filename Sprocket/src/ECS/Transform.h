#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "ECS/Component.h"

#include "Events/Event.h"
#include "Events/ECSEvent.h"

#include <vector>
#include <map>
#include <queue>
#include <stdexcept>
#include <functional>

namespace Sprocket {
  
  class Transform {
    friend class Scene; 
    private:

      std::function<void(Event&)> m_EventCallback;

      std::vector<TransformComponent> m_LocalTransforms;

      // Maps an entityID to the correct index of its Transform component in the vector
      std::map<unsigned int, unsigned int> m_EntityMapping; 
      
      // A priority queue where the top element is the next free slot in the transform vector
      std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_FreeTransforms;

      Transform();

      bool RegisterNewTransform(unsigned int entityID, TransformComponent& tcomp);
      TransformComponent GetTransform(unsigned int entityID) const;
      bool UpdateTransform(unsigned int entityID, TransformComponent& tcomp);
      bool DeleteTransform(unsigned int entityID);

      // Functions the same as OnEvent, but is to be called specifically in the case that the event 
      // given to OnEvent is an ECSEvent
      void OnECSEvent(ECSEvent& event);

      unsigned int GetFreeSlot();

    public:

      /// @brief Handles incoming events.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);

      void RegsisterEventCallback(const std::function<void(Event&)> eventCallback);

  };

}

#endif