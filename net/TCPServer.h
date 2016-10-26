//
//  TCPServer.h
//  WukongBase
//
//  Created by Xuhui on 16/7/8.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef TCPServer_h
#define TCPServer_h

#include "net/IPAddress.h"
#include "net/TCPAcceptor.h"
#include "net/TCPSession.h"
#include <unordered_map>

namespace WukongBase {

namespace Base {
class MessageLoop;
}

namespace Net {

class Packet;

class TCPServer {
public:
    typedef std::function<void(const std::shared_ptr<TCPSession>&)> ConnectCallback;
    typedef std::function<void()> StopCallback;
    
    TCPServer(Base::MessageLoop* messageLoop, const IPAddress& listenAddress, int threadNum = 4);
    ~TCPServer();
    
    void start();
    void stop();
    bool isStarted() { return isStarted_; }
    
     void setConnectCallback(const ConnectCallback& cb)
    {
        connectCallback_ = cb;
    }
    
    void setStopCallback(const StopCallback& cb)
    {
        stopCallback_ = cb;
    }
    
private:
    
    void didConnectComplete(const std::shared_ptr<TCPSocket>& socket);
    Base::MessageLoop* messageLoop_;
    IPAddress listenAddress_;
    int threadNum_;
    ConnectCallback connectCallback_;
    StopCallback stopCallback_;
    std::shared_ptr<TCPAcceptor> acceptor_;
    bool isStarted_;
    static std::unordered_map<TCPSession*, std::shared_ptr<TCPSession>> sessions_;
    
};
}
}

#endif /* TCPServer_h */
