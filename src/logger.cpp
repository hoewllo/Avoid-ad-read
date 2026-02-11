#include "logger.h"
#include <mutex>

using namespace std;

namespace Logger {
    
    // 全局配置
    static Config globalConfig;
    static mutex logMutex;
    
    Config& getConfig() {
        return globalConfig;
    }
    
    void setLevel(Level level) {
        globalConfig.minLevel = level;
    }
    
    // LogStream 实现
    LogStream::LogStream(Level level, const char* file, int line) 
        : level(level), file(file), line(line) {
    }
    
    LogStream::~LogStream() {
        if (level >= globalConfig.minLevel) {
            writeLog();
        }
    }
    
    void LogStream::writeLog() {
        lock_guard<mutex> lock(logMutex);
        
        ostream* output = (level >= Level::ERROR) ? 
                         globalConfig.errorOutput : globalConfig.output;
        
        // 设置颜色（仅对支持ANSI转义的控制台）
        string colorCode = getColorCode();
        if (!colorCode.empty()) {
            *output << colorCode;
        }
        
        // 时间戳
        if (globalConfig.showTimestamp) {
            *output << getTimestamp() << " ";
        }
        
        // 日志级别
        if (globalConfig.showLevel) {
            *output << "[" << getLevelString() << "] ";
        }
        
        // 文件信息（用于调试）
        if (globalConfig.showFileInfo && file && *file) {
            *output << "(" << file;
            if (line > 0) {
                *output << ":" << line;
            }
            *output << ") ";
        }
        
        // 日志消息
        *output << buffer.str();
        
        // 重置颜色
        if (!colorCode.empty()) {
            resetColor();
        }
        
        *output << endl;
    }
    
    string LogStream::getTimestamp() const {
        auto now = chrono::system_clock::now();
        auto time = chrono::system_clock::to_time_t(now);
        auto ms = chrono::duration_cast<chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        tm localTime;
        #ifndef _WIN32
        localtime_s(&localTime, &time);
        #elif defined(__unix__)
        localtime_r(&time, &localTime);
        #endif
        
        ostringstream oss;
        oss << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        oss << "." << setfill('0') << setw(3) << ms.count();
        
        return oss.str();
    }
    
    string LogStream::getLevelString() const {
        switch (level) {
            case Level::DEBUG:   return "DEBUG";
            case Level::INFO:    return "INFO";
            case Level::WARNING: return "WARN";
            case Level::ERROR:   return "ERROR";
            case Level::FATAL:   return "FATAL";
            default:             return "UNKNOWN";
        }
    }
    
    string LogStream::getColorCode() const {
        // ANSI颜色代码
        switch (level) {
            case Level::DEBUG:   return "\033[36m";  // 青色
            case Level::INFO:    return "\033[32m";  // 绿色
            case Level::WARNING: return "\033[33m";  // 黄色
            case Level::ERROR:   return "\033[31m";  // 红色
            case Level::FATAL:   return "\033[35m";  // 洋红色
            default:             return "";
        }
    }
    
    void LogStream::resetColor() const {
        // 重置ANSI颜色
        ostream* output = (level >= Level::ERROR) ? 
                         globalConfig.errorOutput : globalConfig.output;
        *output << "\033[0m";
    }
}