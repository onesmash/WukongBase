//
//  Thread.cpp
//  AppCore
//
//  Created by Xuhui on 15/5/10.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#include "base/thread/Thread.h"
#include "base/message_loop/MessageLoop.h"

namespace WukongBase {

namespace Base {

Thread::Thread(const std::string& name):
    name_(name),
    thread_(NULL),
    messageLoop_(NULL),
    started_(false),
    mutex_(),
    cv_()
{
    
}
    
Thread::~Thread()
{
    if(thread_->joinable()) {
       stop();
    }
}
    
bool Thread::start()
{
    thread_ = std::shared_ptr<std::thread>(new std::thread(&Thread::threadMain, this));
    
    std::unique_lock<std::mutex> lock(mutex_);
    
    cv_.wait(lock);
    started_ = true;
    
    return true;
    
}
    
void Thread::stop()
{
    if(!started_ && !messageLoop_) return;
    if(messageLoop_->running()) {
        messageLoop_->postTask(std::bind(&Thread::stopMessageLoop, this));
    }
    started_ = false;
}
    
void Thread::join()
{
    thread_->join();
}
    
void Thread::stopMessageLoop()
{
    messageLoop_->quite();
}
    
void Thread::threadMain()
{
    messageLoop_ = new MessageLoop();
    cv_.notify_one();
    
    messageLoop_->run();
}
    
}

}