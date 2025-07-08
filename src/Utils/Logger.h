#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>

// 便捷的日志宏定义，自动包含文件名和行号
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg, __FILE__, __LINE__)
#define LOG_INFO(msg) Logger::getInstance().info(msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Logger::getInstance().warning(msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::getInstance().error(msg, __FILE__, __LINE__)
#define LOG_CRITICAL(msg) Logger::getInstance().critical(msg, __FILE__, __LINE__)

class Logger {
public:
    enum class LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
    };

    // 获取单例实例
    static Logger& getInstance();

    // 删除拷贝构造函数和赋值操作符
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // 设置日志级别
    void setLogLevel(LogLevel level);
    
    // 获取当前日志级别
    LogLevel getLogLevel() const { return m_minLevel; }

    // 设置是否输出到文件
    void setLogToFile(bool enable, const std::string& filename = "");

    // 设置是否输出到控制台
    void setLogToConsole(bool enable);

    // 记录日志
    void log(LogLevel level, const std::string& message, const char* file = nullptr, int line = 0);
    void debug(const std::string& message, const char* file = nullptr, int line = 0);
    void info(const std::string& message, const char* file = nullptr, int line = 0);
    void warning(const std::string& message, const char* file = nullptr, int line = 0);
    void error(const std::string& message, const char* file = nullptr, int line = 0);
    void critical(const std::string& message, const char* file = nullptr, int line = 0);

private:
    Logger();
    ~Logger();

    std::string getCurrentTimestamp() const;
    std::string levelToString(LogLevel level) const;
    
    LogLevel m_minLevel;
    bool m_logToFile;
    bool m_logToConsole;
    std::string m_filename;
    std::ofstream m_fileStream;
    std::mutex m_mutex;
};

#endif // LOGGER_H
