#ifndef WINDOWEVENTS_H
#define WINDOWEVENTS_H

#include "Event.h"

namespace Sprocket {

  class WindowCloseEvent : public Event {
    public:
      WindowCloseEvent() {
        this->SetType(WINDOW_CLOSE);
      }
  };

}

#endif