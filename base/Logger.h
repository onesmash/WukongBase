//
//  Logger.h
//  WukongBase
//
//  Created by Xuhui on 16/8/31.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef Logger_h
#define Logger_h

#include <spdlog/spdlog.h>

namespace WukongBase {

namespace Base {
    
class Logger {
public:
    enum LoggerType {
        kLoggerTypeStdout,
        kLoggerTypeStderr,
        kLoggerTypeRotate
    };
    
    enum LogLevel {
        kLogLevelTrace = spdlog::level::level_enum::trace,
        kLogLevelDebug = spdlog::level::level_enum::debug,
        kLogLevelInfo = spdlog::level::level_enum::info,
        kLogLevelWarn = spdlog::level::level_enum::warn,
        kLogLevelError = spdlog::level::level_enum::err,
        kLogLevelCritical = spdlog::level::level_enum::critical
    };
    
    Logger(const std::string& path, size_t maxLogSize, size_t maxFiles, LogLevel level = kLogLevelInfo);
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    ~Logger();
    
    void setLogLevel(LogLevel level)
    {
        logger_->set_level((spdlog::level::level_enum)level);
    }
    
    void flush() const
    {
        logger_->flush();
    }
    
    template <typename... Args> void trace(const char* fmt, const Args&... args) const
    {
        logger_->trace(fmt, args...);
    }
    
    template <typename... Args> void debug(const char* fmt, const Args&... args) const
    {
        logger_->debug(fmt, args...);
    }
    
    template <typename... Args> void info(const char* fmt, const Args&... args) const
    {
        logger_->info(fmt, args...);
    }
    
    template <typename... Args> void warn(const char* fmt, const Args&... args) const
    {
        logger_->warn(fmt, args...);
    }
    
    template <typename... Args> void error(const char* fmt, const Args&... args) const
    {
        logger_->error(fmt, args...);
    }
    
    template <typename... Args> void critical(const char* fmt, const Args&... args) const
    {
        logger_->critical(fmt, args...);
    }
    
    static Logger& sharedStdoutLogger();
    static Logger& sharedStderrLogger();
    
private:
    Logger() {}
    Logger(LoggerType type, LogLevel level = kLogLevelInfo);
    LoggerType type_;
    std::shared_ptr<spdlog::logger> logger_;
    
};
}
}

#define STRING(x) #x
#define LOG_LINE_STRING(x) STRING(x)
#define LOG_TRACE(logger, ...) logger->trace("[" __FILE__ " line #" LOG_LINE_STRING(__LINE__) "] " __VA_ARGS__)
#define LOG_INFO(logger, ...) logger->info(__VA_ARGS__)
#define LOG_WARN(logger, ...) logger->warn(__VA_ARGS__)
#define LOG_ERROR(logger, ...) logger->error(__VA_ARGS__)
#define LOG_CRITICAL(logger, ...) logger->critical(__VA_ARGS__)

#ifdef DEBUG
#define LOG_DEBUG(logger, ...) logger->debug(__VA_ARGS__)
#else
#define LOG_DEBUG(logger, ...)
#endif

#endif /* Logger_h */
