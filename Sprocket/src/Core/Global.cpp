#include "Global.h"

namespace Sprocket {

    namespace Global {

        Logger& FileLogger() {
            static Logger logger("Sprocket.log");
            return logger;
        }

        Logger& ConsoleLogger() {
            static Logger logger;
            return logger;
        }

        float& PixelsPerUnit() {
            static float ppu = 100.0f;
            return ppu;
        }

    }

}