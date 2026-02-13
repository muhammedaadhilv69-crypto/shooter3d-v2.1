#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <ctime>

namespace fps::core {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

class Logger {
public:
    static Logger& GetInstance();
    
    void SetLogLevel(LogLevel level);
    void SetLogFile(const std::string& filename);
    void EnableConsoleOutput(bool enable);
    void EnableFileOutput(bool enable);
    
    void Log(LogLevel level, const std::string& message, const char* file = nullptr, int line = 0);
    void Debug(const std::string& msg, const char* file = nullptr, int line = 0);
    void Info(const std::string& msg, const char* file = nullptr, int line = 0);
    void Warning(const std::string& msg, const char* file = nullptr, int line = 0);
    void Error(const std::string& msg, const char* file = nullptr, int line = 0);
    void Fatal(const std::string& msg, const char* file = nullptr, int line = 0);
    
private:
    Logger();
    ~Logger();
    
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::string GetTimestamp();
    std::string LevelToString(LogLevel level);
    std::string ColorCode(LogLevel level);
    void WriteToConsole(const std::string& msg, LogLevel level);
    void WriteToFile(const std::string& msg);
    
    LogLevel m_minLevel;
    std::ofstream m_fileStream;
    std::mutex m_mutex;
    bool m_consoleOutput;
    bool m_fileOutput;
};

// Macros for easy logging
#ifdef DEBUG
    #define LOG_DEBUG(msg) fps::core::Logger::GetInstance().Debug(msg, __FILE__, __LINE__)
#else
    #define LOG_DEBUG(msg)
#endif

#define LOG_INFO(msg) fps::core::Logger::GetInstance().Info(msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) fps::core::Logger::GetInstance().Warning(msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) fps::core::Logger::GetInstance().Error(msg, __FILE__, __LINE__)
#define LOG_FATAL(msg) fps::core::Logger::GetInstance().Fatal(msg, __FILE__, __LINE__)

} // namespace fps::core
