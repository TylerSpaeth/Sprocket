#ifndef GLOBAL_H
#define GLOBAL_H

#include "Logger/Logger.h"

namespace Sprocket {

    namespace Global {
        
        SPROCKET_API Logger& FileLogger();

        SPROCKET_API Logger& ConsoleLogger();

        SPROCKET_API float& PixelsPerUnit();

    }

}

#endif 
