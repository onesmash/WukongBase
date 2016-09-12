//
//  ThreadPool.h
//  test
//
//  Created by Xuhui on 16/7/14.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef ThreadPool_h
#define ThreadPool_h

#include "base/thread/Thread.h"
#include <vector>

namespace WukongBase {

namespace Base {
    
class ThreadPool {
public:
    ThreadPool(int threadNum);
    ~ThreadPool();
    
    void setThreadNum(int num) { threadNum_ = num; }
    std::shared_ptr<Thread> getThread();
    
    void start();
    void stop();
private:
    int next_;
    int threadNum_;
    std::vector<std::shared_ptr<Thread>> pool_;
};
}
}

#endif /* ThreadPool_h */
