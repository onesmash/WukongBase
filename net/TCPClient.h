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
#include <unordered_map>

namespace WukongBase {
    
namespace Base {
class MessageLoop;
}

namespace Net {
    
class Packet;
    
class TCPClient {
public:
//    typedef std::function<void(const std::shared_ptr<TCPSession>&, std::shared_ptr<Packet>&)> MessageCallback;
//    typedef std::function<void(const std::shared_ptr<TCPSession>&, const Packet&, bool)> WriteCompleteCallback;
    typedef std::function<void(const std::shared_ptr<TCPSession>&)> ConnectCallback;
//    typedef std::function<void()> CloseCallback;
    
    TCPClient(Base::MessageLoop* messageLoop, const IPAddress& serverAddress);
    TCPClient(Base::MessageLoop* messageLoop, const std::string& hostName, uint16_t port);
    ~TCPClient();
    
    void connect();
    void connect(const IPAddress& serverAddress);
    void connect(const std::string& hostName, uint16_t port);
    //void disconnect();
    
    void setConnectCallback(const ConnectCallback& cb)
    {
        connectCallback_ = cb;
    }
    
//    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
//    {
//        writeCompleteCallback_ = cb;
//    }
//    
//    void setMessageCallback(const MessageCallback& cb)
//    {
//        messageCallback_ = cb;
//    }
//    
//    void setCloseCallback(const CloseCallback& cb)
//    {
//        closeCallback_ = cb;
//    }
    
private:
    
    void didConnectComplete(const std::shared_ptr<TCPSocket>& socket);
    
    ConnectCallback connectCallback_;
//    WriteCompleteCallback writeCompleteCallback_;
//    MessageCallback messageCallback_;
//    CloseCallback closeCallback_;
    
    std::shared_ptr<TCPConnector> connector_;
    static std::mutex lock_;
    static std::unordered_map<TCPSession*, std::shared_ptr<TCPSession>> sessions_;
    //std::shared_ptr<TCPSession> session_;
};
}
}

#endif /* TCPClient_h */
