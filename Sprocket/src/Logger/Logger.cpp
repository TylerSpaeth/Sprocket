#include "Logger.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    Logger::Logger(bool useANSIColors) : m_LogFile(stdout), m_UseANSIColors(useANSIColors) {}

    Logger::Logger(const char* filepath) : m_UseANSIColors(false) {
        m_LogFile = fopen(filepath, "w");
        if (!m_LogFile) {
            std::println("Failed to open log file for writing: {}", filepath);
            m_LogFile = stdout;
        }
    }

    Logger::~Logger() {
        if (m_LogFile && m_LogFile != stdout) {
            fclose(m_LogFile);
        }
    }

    void Logger::Info(const std::string& message) {
        WriteLog(INFO_COLOR, INFO, message);
    }
    void Logger::Warning(const std::string& message) {
        WriteLog(WARNING_COLOR, WARNING, message);
    }
    void Logger::Error(const std::string& message) {
        WriteLog(ERROR_COLOR, ERROR, message);
    }   
    void Logger::Debug(const std::string& message) {
        #ifdef _DEBUG
            WriteLog(DEBUG_COLOR, DEBUG, message);
        #endif
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Logger::WriteLog(const std::string& color, const std::string& logTypeString, const std::string& message) {
        auto timestamp = std::chrono::system_clock::now();
        // We only want to add color if we are logging to stdout.
        if (m_UseANSIColors && m_LogFile == stdout) {
            std::print(stdout, "{}", color);
        }
        std::println(m_LogFile, "{:%Y-%m-%d %H:%M:%S} {} : {}", timestamp, logTypeString, message);
        fflush(m_LogFile);

        // If we are in a debug build, we will log everything to stdout
        #ifdef _DEBUG
            if (m_LogFile != stdout) {
                if (m_UseANSIColors) {
                    std::print(stdout, "{}",color);
                }
                std::println(stdout, "Logged to file: {:%Y-%m-%d %H:%M:%S} {} : {}", timestamp, logTypeString, message);
            }
        #endif
    }
}