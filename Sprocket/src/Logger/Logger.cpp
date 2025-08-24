#include "Logger.h"

namespace Sprocket {

    Logger::Logger(const std::string& filepath) {
        m_LogFile = fopen(filepath.c_str(), "w");
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

    void Logger::WriteLog(const std::string& logTypeString, const std::string& message) {
        auto timestamp = std::chrono::system_clock::now();
        std::println(m_LogFile, "{:%Y-%m-%d %H:%M:%S} {} : {}", timestamp, logTypeString, message);
        fflush(m_LogFile);
    }

    void Logger::Info(const std::string& message) {
        WriteLog(INFO, message);
    }
    void Logger::Warning(const std::string& message) {
        WriteLog(WARNING, message);
    }
    void Logger::Error(const std::string& message) {
        WriteLog(ERROR, message);
    }   
    void Logger::Debug(const std::string& message) {
        #ifdef _DEBUG
            WriteLog(DEBUG, message);
        #endif
    }

}