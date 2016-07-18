//
//  TCPClient.h
//  Wukong
//
//  Created by Xuhui on 16/7/4.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef TCPClient_h
#define TCPClient_h

#include "net/IPAddress.h"
#include "net/TCPConnector.h"
#include "net/TCPSession.h"
#include <string>

namespace WukongBase {
    
namespace Base {
class MessageLoop;
class IOBuffer;
}

namespace Net {
    
class TCPClient {
public:
    typedef std::function<void(const std::shared_ptr<TCPSession>&, const std::shared_ptr<Base::IOBuffer>&)> MessageCallback;
    typedef std::function<void(const std::shared_ptr<TCPSession>&, bool)> WriteCompleteCallback;
    typedef std::function<void(const std::shared_ptr<TCPSession>&)> ConnectCallback;
    typedef std::function<void()> CloseCallback;
    
    TCPClient(Base::MessageLoop* messageLoop, const IPAddress& serverAddress);
    TCPClient(Base::MessageLoop* messageLoop, const std::string& hostName, uint16_t port);
    ~TCPClient();
    
    void connect();
    void connect(const IPAddress& serverAddress);
    void connect(const std::string& hostName, uint16_t port);
    void disconnect();
    
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
    
    void setCloseCallback(const CloseCallback& cb)
    {
        closeCallback_ = cb;
    }
    
private:
    
    void didConnectComplete(const std::shared_ptr<TCPSocket>& socket);
    void didReadComplete(const std::shared_ptr<Base::IOBuffer>& buffer);
    void didWriteComplete(bool success);
    void didCloseComplete();
    
    ConnectCallback connectCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
    
    std::shared_ptr<TCPConnector> connector_;
    std::shared_ptr<TCPSession> session_;
};
}
}

#endif /* TCPClient_h */
