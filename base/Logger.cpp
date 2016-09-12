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

Logger::Logger(LoggerType type): type_(type)
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
}

Logger::Logger(const std::string& path, size_t maxLogSize, size_t maxFiles): type_(kLoggerTypeRotate)
{
    logger_ = spdlog::get(path);
    if(logger_ == nullptr) {
        logger_ = spdlog::rotating_logger_mt(path, path, maxLogSize, maxFiles);
        logger_->set_pattern("[%x %H:%M:%S:%e] [%l] %v");
    }
}

Logger::~Logger()
{
    logger_->flush();
}

Logger& Logger::sharedStdoutLogger()
{
    static Logger logger(kLoggerTypeStdout);
    return logger;
}

Logger& Logger::sharedStderrLogger()
{
    static Logger logger(kLoggerTypeStderr);
    return logger;
}