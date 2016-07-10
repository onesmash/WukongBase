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

namespace WukongBase {
    
namespace Base {
class MessageLoop;
class IOBuffer;
}

namespace Net {
    
class TCPClient {
public:
    typedef std::function<void(const std::shared_ptr<TCPSession>&, std::shared_ptr<Base::IOBuffer>&)> MessageCallback;
    typedef std::function<void(std::shared_ptr<TCPSession>&, bool)> WriteCompleteCallback;
    typedef std::function<void(const std::shared_ptr<TCPSession>&)> ConnectCallback;
    
    TCPClient(Base::MessageLoop* messageLoop, const IPAddress& serverAddress);
    ~TCPClient();
    
    void connect();
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
    
private:
    
    void didConnectComplete(const std::shared_ptr<TCPSocket>& socket);
    void didReadComplete(std::shared_ptr<Base::IOBuffer>& buffer);
    void didWriteComplete(bool success);
    void didCloseComplete();
    
    ConnectCallback connectCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    MessageCallback messageCallback_;
    
    std::shared_ptr<TCPConnector> connector_;
    std::shared_ptr<TCPSession> session_;
};
}
}

#endif /* TCPClient_h */
