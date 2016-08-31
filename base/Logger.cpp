//
//  Logger.cpp
//  WukongBase
//
//  Created by Xuhui on 16/8/31.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "Logger.h"
#include "Thread.h"
#include <memory>

using namespace WukongBase::Base;

#define kStdoutLoggerTLSKey "kStdoutLoggerTLSKey"
#define kStderrLoggerTLSKey "kStderrLoggerTLSKey"

Logger::Logger(LoggerType type): type_(type)
{
    switch (type_) {
        case kLoggerTypeStdout: {
            logger_ = spdlog::stdout_logger_mt("StdoutLogger");
        } break;
        case kLoggerTypeStderr: {
            logger_ = spdlog::stderr_logger_mt("StderrLogger");
        } break;
        default:
            break;
    }
}

Logger::~Logger()
{
    
}

const Logger& Logger::sharedStdoutLogger()
{
    const std::shared_ptr<Logger>& logger = Thread::getThreadLocal<Logger>(kStdoutLoggerTLSKey);
    if(logger == nullptr) {
        Logger* l = new Logger(kLoggerTypeStdout);
        Thread::setThreadLocal<Logger>(kStdoutLoggerTLSKey, l);
        return *l;
    } else {
        return *logger;
    }
}

const Logger& Logger::sharedStderrLogger()
{
    const std::shared_ptr<Logger>& logger = Thread::getThreadLocal<Logger>(kStderrLoggerTLSKey);
    if(logger == nullptr) {
        Logger* l = new Logger(kLoggerTypeStderr);
        Thread::setThreadLocal<Logger>(kStderrLoggerTLSKey, l);
        return *l;
    } else {
        return *logger;
    }
}