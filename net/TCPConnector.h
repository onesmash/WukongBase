//
//  TCPConnector.h
//  Wukong
//
//  Created by Xuhui on 16/7/4.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef TCPConnector_h
#define TCPConnector_h

#include "net/IPAddress.h"
#include "net/TCPSocket.h"
#include <string>

namespace WukongBase {
    
namespace Base {
class MessageLoop;
}

namespace Net {
    
class TCPConnector {
public:
    typedef std::function<void(const std::shared_ptr<TCPSocket>&)> NewTCPSessionCallback;
    
    TCPConnector(Base::MessageLoop* messageLoop, const IPAddress& serverAddress);
    TCPConnector(Base::MessageLoop* messageLoop, const std::string& hostName, uint16_t port);
    ~TCPConnector();
    
    void connect();
    
    void setNewTCPSessionCallback(const NewTCPSessionCallback& cb)
    {
        newTCPSessionCallback_ = cb;
    }
    
private:
    void connectInLoop();
    void resolveInLoop(const std::string& hostName, uint16_t port);
    
    void didConnectComplete(bool success);
    
    NewTCPSessionCallback newTCPSessionCallback_;
    
    Base::MessageLoop* messageLoop_;
    IPAddress serverAddress_;
    std::shared_ptr<TCPSocket> socket_;
};
}
}

#endif /* TCPConnector_h */
