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

namespace WukongBase {

namespace Base {
class MessageLoop;
}

namespace Net {

class Packet;

class TCPServer {
public:
    typedef std::function<void(const std::shared_ptr<TCPSession>&, std::shared_ptr<Packet>&)> MessageCallback;
    typedef std::function<void(const std::shared_ptr<TCPSession>&)> ConnectCallback;
    typedef std::function<void(const std::shared_ptr<TCPSession>&, const Packet&, bool)> WriteCompleteCallback;
    typedef std::function<void()> StopCallback;
    
    TCPServer(Base::MessageLoop* messageLoop, const IPAddress& listenAddress, int threadNum = 4);
    ~TCPServer();
    
    void start();
    void stop();
    
     void setConnectCallback(const ConnectCallback& cb)
    {
        connectCallback_ = cb;
    }
    
    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    {
        writeCompleteCallback_ = cb;
    }
    
    void setMessageCallback(const MessageCallback& cb)
    {
        messageCallback_ = cb;
    }
    
    void setStopCallback(const StopCallback& cb)
    {
        stopCallback_ = cb;
    }
    
private:
    
    void didConnectComplete(const std::shared_ptr<TCPSocket>& socket);
    void didReadComplete(const std::shared_ptr<TCPSession>&, std::shared_ptr<Packet>& buffer);
    void didWriteComplete(const std::shared_ptr<TCPSession>&, const Packet& packet, bool success);
    
    ConnectCallback connectCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    MessageCallback messageCallback_;
    StopCallback stopCallback_;
    
    std::shared_ptr<TCPAcceptor> acceptor_;
    
};
}
}

#endif /* TCPServer_h */
