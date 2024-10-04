#ifndef EVENTHANDER_H
#define EVENTHANDLER_H

#include "Event.h"
#include <functional>

namespace Sprocket {

  class EventHandler {
    private:
      Event& m_Event;
    public:
      EventHandler(Event& event) : m_Event(event){}

      // TODO should add some sort of type checking here
      template<typename Function>
      void Post(const Function& function) {
        function(m_Event);
      }
  };

}

#endif