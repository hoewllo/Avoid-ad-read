#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace Logger {
    // 日志级别
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };
    
    // 日志配置
    struct Config {
        Level minLevel = Level::INFO;
        bool showTimestamp = true;
        bool showLevel = true;
        bool showFileInfo = false;
        std::ostream* output = &std::cout;
        std::ostream* errorOutput = &std::cerr;
    };
    
    // 获取全局配置
    Config& getConfig();
    
    // 设置日志级别
    void setLevel(Level level);
    
    // 日志流类
    class LogStream {
    public:
        LogStream(Level level, const char* file = "", int line = 0);
        ~LogStream();
        
        template<typename T>
        LogStream& operator<<(const T& value) {
            buffer << value;
            return *this;
        }
        
    private:
        Level level;
        std::stringstream buffer;
        const char* file;
        int line;
        
        void writeLog();
        std::string getTimestamp() const;
        std::string getLevelString() const;
        std::string getColorCode() const;
        void resetColor() const;
    };
    
    // 日志宏
    #define LOG_DEBUG Logger::LogStream(Logger::Level::DEBUG, __FILE__, __LINE__)
    #define LOG_INFO Logger::LogStream(Logger::Level::INFO, __FILE__, __LINE__)
    #define LOG_WARN Logger::LogStream(Logger::Level::WARNING, __FILE__, __LINE__)
    #define LOG_ERROR Logger::LogStream(Logger::Level::ERROR, __FILE__, __LINE__)
    #define LOG_FATAL Logger::LogStream(Logger::Level::FATAL, __FILE__, __LINE__)
}

#endif // LOGGER_H