//
//  Logger.cpp
//  WukongBase
//
//  Created by Xuhui on 16/8/31.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "Logger.h"
#include <memory>

using namespace WukongBase::Base;

#define kStdoutLoggerName "kStdoutLogger"
#define kStderrLoggerName "kStderrLogger"

Logger::Logger(LoggerType type, LogLevel level): type_(type)
{
    switch (type_) {
        case kLoggerTypeStdout: {
            logger_ = spdlog::get(kStdoutLoggerName);
            if(logger_ == nullptr) {
                logger_ = spdlog::stdout_logger_mt(kStdoutLoggerName);
                logger_->set_pattern("[%x %H:%M:%S:%e] [%l] %v");
            }
        } break;
        case kLoggerTypeStderr: {
            logger_ = spdlog::get(kStderrLoggerName);
            if(logger_ == nullptr) {
                logger_ = spdlog::stdout_logger_mt(kStderrLoggerName);
                logger_->set_pattern("[%x %H:%M:%S:%e] [%l] %v");
            }
        } break;
        default:
            break;
    }
    logger_->set_level((spdlog::level::level_enum)level);
}

Logger::Logger(const std::string& path, size_t maxLogSize, size_t maxFiles, LogLevel level): type_(kLoggerTypeRotate)
{
    logger_ = spdlog::get(path);
    if(logger_ == nullptr) {
        logger_ = spdlog::rotating_logger_mt(path, path, maxLogSize, maxFiles);
        logger_->set_pattern("[%x %H:%M:%S:%e] [%l] %v");
    }
    logger_->set_level((spdlog::level::level_enum)level);
}

Logger::~Logger()
{
    logger_->flush();
}

Logger& Logger::sharedStdoutLogger()
{
#ifdef DEBUG
    static Logger logger(kLoggerTypeStdout, kLogLevelTrace);
#else
    static Logger logger(kLoggerTypeStdout);
#endif
    return logger;
}

Logger& Logger::sharedStderrLogger()
{
#ifdef DEBUG
    static Logger logger(kLoggerTypeStdout, kLogLevelTrace);
#else
    static Logger logger(kLoggerTypeStderr);
#endif
    return logger;
}