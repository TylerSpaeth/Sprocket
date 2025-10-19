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

        static inline const std::string INFO_COLOR = "\033[37m";
        static inline const std::string WARNING_COLOR = "\033[33m";
        static inline const std::string ERROR_COLOR = "\033[31m";
        static inline const std::string DEBUG_COLOR = "\033[36m";

        static inline const std::string INFO = "   Info";
        static inline const std::string WARNING = "Warning";
        static inline const std::string ERROR = "  Error";
        static inline const std::string DEBUG = "  Debug";

        bool m_UseANSIColors;

        FILE* m_LogFile;

    public:

        /// @brief Create a logger that logs to stdout.
        /// @param useANSIColors Whether or not ansi escape characters should be used to color the 
        /// log messages from this logger. The is enabled by default.
        Logger(bool useANSIColors = true);

        /// @brief A logger that logs to the given filepath. Note that in debug builds this will 
        /// also duplicate all logs to stdout.
        Logger(const char* filepath);

        ~Logger();

        /// @brief Writes a log at the debug level.
        /// @param message - The message to log
        void Info(const std::string& message);

        /// @brief Writes a log at the warning level.
        /// @param message - The message to log
        void Warning(const std::string& message);

        /// @brief Writes a log at the error level.
        /// @param message - The message to log
        void Error(const std::string& message);

        /// @brief Writes a log at the debug level. Note: Debug messages are only logged in debug 
        /// builds to ensure that release logs do not get cluttered.
        /// @param message - The message to log
        void Debug(const std::string& message);

    private:

        /// @brief Writes a message with the with a log type and timestamp to the logfile.
        /// @param logTypeString - Represents the type of log this is.
        /// @param message - The actual message of the log
        void WriteLog(const std::string& color, const std::string& logTypeString, const std::string& message);
    };
}

#endif