#ifndef GLOBAL_H
#define GLOBAL_H

#include "Logger/Logger.h"

namespace Sprocket {

    namespace Global {
        
        inline static Logger fileLogger("Sprocket.log");
        inline static Logger consoleLogger;

        inline static float pixelsPerUnit = 100.0f;

    }

}

#endif 
