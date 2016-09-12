//
//  MessageLoop.h
//  AppCore
//
//  Created by Xuhui on 15/5/10.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#ifndef __Base__MessageLoop__
#define __Base__MessageLoop__
#include "uv.h"
#include "base/message_loop/Closure.h"
#include "base/message_loop/Time.h"
#include "base/message_loop/Task.h"
#include "base/message_loop/ConcurrentTaskQueue.h"
#include <memory>
#include <unordered_map>
#include <list>

namespace WukongBase {

namespace Base {
    
class MessageLoop {
    
public:
    
    typedef uv_loop_t EventLoop;
    
    MessageLoop();
    
    virtual ~MessageLoop();
    
    static MessageLoop* current();
    
    EventLoop& eventLoop() { return eventLoop_; }
    
    bool running() { return running_; }
    
    void postTask(const Closure& closure);
    
    void postDelayTask(const Closure& closure, const TimeDelta& delayTime);
    
    void scheduleWork(bool wasEmpty);
    
    void scheduleDelayedWork();
    
    void breakMessageLoop();
    
    char* wakeUpPipeMessageBuf() { return wakeUpPipeMessageBuf_; }
    
    size_t taskQueueSize() const { return taskQueue_.size(); }
    
    size_t pendingTaskQueueSize() const { return pendingTaskQueue_->size(); }
    
    void quite();
    
    bool attachToCurrentThread();
    
//    void addObserver(MessageLoopObserver& observer);
//    
//    void removeObserver(const MessageLoopObserver& observer);
    
    TimePoint recentNow_;
    
    void run();
    
protected:
    
    
private:
    
    friend class Thread;
    
    void loadPendingTask();
    
    void runTask(Task& task);
    
    bool doWork();
    
    bool doDelayedWork();
    
    EventLoop eventLoop_;
    
    bool running_;
    
    std::shared_ptr<ConcurrentTaskQueue> pendingTaskQueue_;
    
    TaskQueue taskQueue_;
    DelayedTaskQueue delayedTaskQueue_;
    int wakeUpPipeInId_;
    int wakeUpPipeOutId_;
    uv_pipe_t wakeUpPipeOut_;
    char wakeUpPipeMessageBuf_[2];
    uv_timer_t timer_;
    
    std::mutex mutex_;
    
};
    
}
    
}

#endif /* defined(__Base__MessageLoop__) */
