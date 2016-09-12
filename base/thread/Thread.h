//
//  Thread.h
//  AppCore
//
//  Created by Xuhui on 15/5/10.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#ifndef __Base__Thread__
#define __Base__Thread__

#include <string>
#include <thread>
#include <memory>
#include <unordered_map>

namespace WukongBase {
    
namespace Base {
    
class MessageLoop;
    
class Thread {
    
public:
    explicit Thread(const std::string& name);
    
    virtual ~Thread();
    
    bool start();
    
    void stop();
    
    void join();
    
    const std::string& name() const { return name_; }
    
    MessageLoop* messageLoop() const {
        return messageLoop_;
    }
    
    static void sleep(double seconds);
    
    template<typename T>
    static void setThreadLocal(const std::string& key, T* value)
    {
        std::unordered_map<std::string, std::shared_ptr<void>>* storage = Thread::getThreadLocalStorage();
        if(storage) {
            storage->insert({key, std::shared_ptr<T>(value)});
        }
    }
    
    template<typename T>
    static std::shared_ptr<T> getThreadLocal(const std::string& key)
    {
        std::unordered_map<std::string, std::shared_ptr<void>>* storage = Thread::getThreadLocalStorage();
        if(storage == nullptr) {
            return nullptr;
        }
        auto iter = storage->find(key);
        if(iter != storage->end()) {
            return std::static_pointer_cast<T>(iter->second);
        }
        return nullptr;
    }
    
    static void deleteThreadLocal(const std::string& key)
    {
        std::unordered_map<std::string, std::shared_ptr<void>>* storage = Thread::getThreadLocalStorage();
        if(storage) {
            storage->erase(key);
        }
    }
   
private:
    
    static std::unordered_map<std::string, std::shared_ptr<void>>* getThreadLocalStorage();
    
    friend class MessageLoop;
    
    friend class std::thread;
    
    void threadMain();
    
    void stopMessageLoop();
    
    std::string name_;
    
    std::shared_ptr<std::thread> thread_;
    
    MessageLoop* messageLoop_;
    
    bool started_;
    
    std::mutex mutex_;
    std::condition_variable cv_;
    
};
    
}
    
}

#endif /* defined(__Base__Thread__) */
