//
//  TCPSession.h
//  AppCore
//
//  Created by Xuhui on 15/5/31.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#ifndef __Net__TCPSession__
#define __Net__TCPSession__

#include "net/TCPSocket.h"
#include "net/IPAddress.h"
#include <thread>

namespace WukongBase {
namespace Net {
    
class Packet;
    
class TCPSession {
    
public:
    typedef std::function<void(std::shared_ptr<Packet>&)> ReadCompleteCallback;
    typedef std::function<void(const Packet& packet, bool)> WriteCompleteCallback;
    typedef std::function<void(bool)> CloseCallback;
    TCPSession(const std::shared_ptr<TCPSocket>& socket, const IPAddress& localAddress, const IPAddress& peerAddress);
    
    virtual ~TCPSession();
    
    void send(const Packet& packet);
    void send(Packet&& packet);
    
    void startRead();
    void stopRead();
    
    void shutdown();
    void close();
    
    bool isClosed();
    
    const IPAddress& getLocalAddress() const
    {
        return localAddress_;
    }
    
    const IPAddress& getPeerAddress() const
    {
        return peerAddress_;
    }
    
    void setReadCompleteCallback(const ReadCompleteCallback& cb)
    {
        readCompleteCallback_ = cb;
    }
    
    void setWriteCompleteCallback(const WriteCompleteCallback& cb)
    {
        writeCompleteCallback_ = cb;
    }
    
    void setCloseCallback(const CloseCallback& cb)
    {
        closeCallback_ = cb;
    }
    
private:
    
    enum State { kDisconnected, kConnecting, kConnected, kDisconnecting };
    
    void setState(State state) { state_ = state; }
    
    std::mutex lock_;
    
    IPAddress localAddress_;
    IPAddress peerAddress_;
    
    State state_;
    
    ReadCompleteCallback readCompleteCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    CloseCallback closeCallback_;
    
    std::shared_ptr<TCPSocket> socket_;
    
};
    
}
}

#endif /* defined(__Net__TCPSession__) */
