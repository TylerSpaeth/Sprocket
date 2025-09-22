#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H

#include "Event.h"

namespace Sprocket {

    class WindowResizedEvent : public Event {
    private:
        int xDimension;
        int yDimension;
    public:
        WindowResizedEvent(int xDimension, int yDimension) : xDimension(xDimension), yDimension(yDimension) {
            this->SetType(EventType::WINDOW_RESIZED);
        }

        int GetXDimension() const { return xDimension; }
        int GetYDimension() const { return yDimension; }
    };

}

#endif