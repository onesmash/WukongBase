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
    
    Logger(const std::string& path, size_t maxLogSize, size_t maxFiles);
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    ~Logger();
    
    void flush()
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
    Logger(LoggerType type);
    LoggerType type_;
    std::shared_ptr<spdlog::logger> logger_;
    
};
}
}


#endif /* Logger_h */
