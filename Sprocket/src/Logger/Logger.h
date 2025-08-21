#ifndef LOGGER_H
#define LOGGER_H

#include "Core/Sprocket.pch"

#include "Core/Macros.h"

namespace Sprocket {
    /// @brief The Logger class is used to log messages to the console or a file.
    /// It provides various logging levels such as Info, Warning, Error, and Debug.
    /// Unlike other pieces of Sprocket, this will not run through the event system,
    /// as it is intended to be a low-level utility for debugging and information. 
    class SPROCKET_API Logger {

    private:

        static inline const std::string INFO = "   Info";
        static inline const std::string WARNING = "Warning";
        static inline const std::string ERROR = "  Error";
        static inline const std::string DEBUG = "  Debug";

        FILE* m_LogFile;

        void WriteLog(const std::string& logTypeString, const std::string& message);

    public:
        Logger() : m_LogFile(stdout) {} // Default to stdout
        Logger(const std::string& filepath);
        ~Logger();
        void Info(const std::string& message);
        void Warning(const std::string& message);
        void Error(const std::string& message);
        // Debug messages are only logged in debug builds to ensure that release logs do not get cluttered.
        void Debug(const std::string& message);
    };
}

#endif