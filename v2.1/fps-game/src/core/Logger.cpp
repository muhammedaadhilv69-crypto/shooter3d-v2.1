#include "core/Logger.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

namespace fps::core {

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() 
    : m_minLevel(LogLevel::Debug)
    , m_consoleOutput(true)
    , m_fileOutput(false) {
}

Logger::~Logger() {
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
}

void Logger::SetLogLevel(LogLevel level) {
    m_minLevel = level;
}

void Logger::SetLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
    m_fileStream.open(filename, std::ios::out | std::ios::app);
    m_fileOutput = m_fileStream.is_open();
}

void Logger::EnableConsoleOutput(bool enable) {
    m_consoleOutput = enable;
}

void Logger::EnableFileOutput(bool enable) {
    m_fileOutput = enable;
}

std::string Logger::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error:   return "ERROR";
        case LogLevel::Fatal:   return "FATAL";
        default:                return "UNKNOWN";
    }
}

std::string Logger::ColorCode(LogLevel level) {
#ifdef PLATFORM_WINDOWS
    return "";
#else
    switch (level) {
        case LogLevel::Debug:   return "\033[36m"; // Cyan
        case LogLevel::Info:    return "\033[32m"; // Green
        case LogLevel::Warning: return "\033[33m"; // Yellow
        case LogLevel::Error:   return "\033[31m"; // Red
        case LogLevel::Fatal:   return "\033[35m"; // Magenta
        default:                return "\033[0m";
    }
#endif
}

void Logger::WriteToConsole(const std::string& msg, LogLevel level) {
    if (!m_consoleOutput) return;
    
#ifdef PLATFORM_WINDOWS
    std::cout << msg << std::endl;
#else
    std::cout << ColorCode(level) << msg << "\033[0m" << std::endl;
#endif
}

void Logger::WriteToFile(const std::string& msg) {
    if (!m_fileOutput || !m_fileStream.is_open()) return;
    m_fileStream << msg << std::endl;
    m_fileStream.flush();
}

void Logger::Log(LogLevel level, const std::string& message, const char* file, int line) {
    if (level < m_minLevel) return;
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::stringstream ss;
    ss << "[" << GetTimestamp() << "]";
    ss << "[" << LevelToString(level) << "]";
    
    if (file) {
        std::string filepath(file);
        size_t lastSlash = filepath.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            filepath = filepath.substr(lastSlash + 1);
        }
        ss << "[" << filepath << ":" << line << "]";
    }
    
    ss << " " << message;
    
    std::string logMessage = ss.str();
    WriteToConsole(logMessage, level);
    WriteToFile(logMessage);
}

void Logger::Debug(const std::string& msg, const char* file, int line) {
    Log(LogLevel::Debug, msg, file, line);
}

void Logger::Info(const std::string& msg, const char* file, int line) {
    Log(LogLevel::Info, msg, file, line);
}

void Logger::Warning(const std::string& msg, const char* file, int line) {
    Log(LogLevel::Warning, msg, file, line);
}

void Logger::Error(const std::string& msg, const char* file, int line) {
    Log(LogLevel::Error, msg, file, line);
}

void Logger::Fatal(const std::string& msg, const char* file, int line) {
    Log(LogLevel::Fatal, msg, file, line);
}

} // namespace fps::core
