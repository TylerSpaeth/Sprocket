#include "Global.h"

namespace Sprocket {
    Logger Global::fileLogger = Logger("Sprocket.log");
    Logger Global::consoleLogger = Logger();

    float Global::pixelsPerUnit = 100;
}