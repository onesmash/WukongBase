//
//  TCPAcceptor.h
//  WukongBase
//
//  Created by Xuhui on 16/7/8.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef TCPAcceptor_h
#define TCPAcceptor_h

#include "net/IPAddress.h"
#include "net/TCPSocket.h"
#include "base/thread/ThreadPool.h"

namespace WukongBase {

namespace Base {
class MessageLoop;
}

namespace Net {
    
class TCPAcceptor {
public:
    typedef std::function<void(const std::shared_ptr<TCPSocket>&)> NewTCPSessionCallback;
    typedef std::function<void()> StopCallback;
    
    TCPAcceptor(Base::MessageLoop* messageLoop, const IPAddress& listenAddress, bool reusePort, int threadNum);
    ~TCPAcceptor();
    
    void listen(int backlog);
    void stop();
    
    void setNewTCPSessionCallback(const NewTCPSessionCallback& cb)
    {
        newTCPSessionCallback_ = cb;
    }
    
    void setStopCallback(const StopCallback& cb)
    {
        stopCallback_ = cb;
    }
    
private:
    
    void didReciveConnectRequest();
    
    void listenInLoop(int backlog);
    
    Base::MessageLoop* messageLoop_;
    std::shared_ptr<TCPSocket> socket_;
    NewTCPSessionCallback newTCPSessionCallback_;
    StopCallback stopCallback_;
    int threadNum_;
    Base::ThreadPool threadPool_;
    
};
}
}

#endif /* TCPAcceptor_h */
