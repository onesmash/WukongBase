//
//  TCPServer.h
//  WukongBase
//
//  Created by Xuhui on 16/7/8.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef TCPServer_h
#define TCPServer_h

#include "IPAddress.h"
#include "TCPAcceptor.h"
#include "TCPSession.h"

namespace WukongBase {

namespace Base {
    class MessageLoop;
    class IOBuffer;
}

namespace Net {
class TCPServer {
public:
    typedef std::function<void(const std::shared_ptr<TCPSession>&, std::shared_ptr<Base::IOBuffer>&)> MessageCallback;
    typedef std::function<void(const std::shared_ptr<TCPSession>&)> ConnectCallback;
    typedef std::function<void(std::shared_ptr<TCPSession>&, bool)> WriteCompleteCallback;
    
    TCPServer(Base::MessageLoop* messageLoop, const IPAddress& listenAddress);
    ~TCPServer();
    
    void start();
    
private:
    
    void didConnectComplete(const std::shared_ptr<TCPSocket>& socket);
    void didReadComplete(std::shared_ptr<TCPSession>&, std::shared_ptr<Base::IOBuffer>& buffer);
    void didWriteComplete(std::shared_ptr<TCPSession>&, bool success);
    
    ConnectCallback connectCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    MessageCallback messageCallback_;
    
    std::shared_ptr<TCPAcceptor> acceptor_;
    
};
}
}

#endif /* TCPServer_h */
