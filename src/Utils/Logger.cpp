#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() 
    : m_minLevel(LogLevel::INFO)
    , m_logToFile(false)
    , m_logToConsole(true) {
}

Logger::~Logger() {
    if (m_fileStream.is_open()) {
        m_fileStream.close();
    }
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_minLevel = level;
}

void Logger::setLogToFile(bool enable, const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logToFile = enable;
    
    if (enable && !filename.empty()) {
        if (m_fileStream.is_open()) {
            m_fileStream.close();
        }
        m_filename = filename;
        m_fileStream.open(filename, std::ios::app);
        if (!m_fileStream.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
            m_logToFile = false;
        }
    } else if (!enable && m_fileStream.is_open()) {
        m_fileStream.close();
    }
}

void Logger::setLogToConsole(bool enable) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logToConsole = enable;
}

void Logger::log(LogLevel level, const std::string& message, const char* file, int line) {
    if (level < m_minLevel) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = levelToString(level);
    std::string location = "";
    
    // 如果提供了文件名和行号，添加位置信息
    if (file && line > 0) {
        // 只取文件名，不包含完整路径
        const char* filename = strrchr(file, '/');
        if (filename) {
            filename++; // 跳过 '/'
        } else {
            filename = file;
        }
        location = " [" + std::string(filename) + ":" + std::to_string(line) + "]";
    }
    
    std::string formattedMessage = "[" + timestamp + "] [" + levelStr + "]" + location + " " + message;
    
    if (m_logToConsole) {
        std::cout << formattedMessage << std::endl;
    }
    
    if (m_logToFile && m_fileStream.is_open()) {
        m_fileStream << formattedMessage << std::endl;
        m_fileStream.flush();
    }
}

void Logger::debug(const std::string& message, const char* file, int line) {
    log(LogLevel::DEBUG, message, file, line);
}

void Logger::info(const std::string& message, const char* file, int line) {
    log(LogLevel::INFO, message, file, line);
}

void Logger::warning(const std::string& message, const char* file, int line) {
    log(LogLevel::WARNING, message, file, line);
}

void Logger::error(const std::string& message, const char* file, int line) {
    log(LogLevel::ERROR, message, file, line);
}

void Logger::critical(const std::string& message, const char* file, int line) {
    log(LogLevel::CRITICAL, message, file, line);
}

std::string Logger::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG:    return "DEBUG";
        case LogLevel::INFO:     return "INFO";
        case LogLevel::WARNING:  return "WARNING";
        case LogLevel::ERROR:    return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}
