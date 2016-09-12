//
//  ThreadPool.cpp
//  test
//
//  Created by Xuhui on 16/7/14.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "base/thread/ThreadPool.h"
#include "base/message_loop/MessageLoop.h"
#include "base/Logger.h"

namespace WukongBase {

namespace Base {
    
ThreadPool::ThreadPool(int threadNum): next_(0), threadNum_(threadNum)
{
}
    
ThreadPool::~ThreadPool()
{
}
    
std::shared_ptr<Thread> ThreadPool::getThread()
{
    if(!pool_.empty()) {
        std::shared_ptr<Thread>& thread = pool_[next_];
        size_t pendingTaskSize = thread->messageLoop()->pendingTaskQueueSize();
        if(pendingTaskSize > 20) {
            Logger::sharedStderrLogger().warn("thread pool - thread#{} too many pending task", next_);
        }
        next_++;
        if(next_ >= threadNum_) {
            next_ = 0;
        }
        return thread;
    } else {
        return std::shared_ptr<Thread>();
    }
}
    
void ThreadPool::start()
{
    for (int i = 0; i < threadNum_; i++) {
        std::shared_ptr<Thread> thread(new Thread(""));
        thread->start();
        pool_.push_back(thread);
    }
}
    
void ThreadPool::stop()
{
    for (std::vector<std::shared_ptr<Thread>>::const_iterator iter = pool_.begin(); iter != pool_.end(); ++iter) {
        (*iter)->stop();
    }
}
}
}
