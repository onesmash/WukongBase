//
//  Thread.cpp
//  AppCore
//
//  Created by Xuhui on 15/5/10.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#include "base/thread/Thread.h"
#include "base/message_loop//Time.h"
#include "base/message_loop/MessageLoop.h"

namespace WukongBase {

namespace Base {
    
static uv_key_t threadLocalStorageKey;

Thread::Thread(const std::string& name):
    name_(name),
    thread_(nullptr),
    messageLoop_(nullptr),
    started_(false),
    mutex_(),
    cv_()
{
    
}
    
Thread::~Thread()
{
    if(thread_ != nullptr && thread_->joinable()) {
        stop();
        thread_->join();
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
    messageLoop_->attachToCurrentThread();
    uv_key_create(&threadLocalStorageKey);
    std::unordered_map<std::string, std::shared_ptr<void>>* storage = new std::unordered_map<std::string, std::shared_ptr<void>>();
    uv_key_set(&threadLocalStorageKey, storage);
    cv_.notify_one();
    
    messageLoop_->run();
}
    
void Thread::sleep(double seconds)
{
    std::this_thread::sleep_for(std::chrono::microseconds((long long)(seconds * kMicroSecondsPerSecond)));
}
    
std::unordered_map<std::string, std::shared_ptr<void>>* Thread::getThreadLocalStorage()
{
    std::unordered_map<std::string, std::shared_ptr<void>>* storage = nullptr;
    if(threadLocalStorageKey > 0) {
        storage = (std::unordered_map<std::string, std::shared_ptr<void>>*)uv_key_get(&threadLocalStorageKey);
    }
    return storage;
}
    
}

}