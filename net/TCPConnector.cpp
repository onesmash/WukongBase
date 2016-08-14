//
//  TCPConnector.cpp
//  Wukong
//
//  Created by Xuhui on 16/7/4.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/TCPConnector.h"
#include "base/message_loop/MessageLoop.h"
#include <vector>

namespace WukongBase {

namespace Net {
    
TCPConnector::TCPConnector(Base::MessageLoop* messageLoop, const IPAddress& serverAddress)
:   messageLoop_(messageLoop),
    serverAddress_(serverAddress)
{

}
    
TCPConnector::TCPConnector(Base::MessageLoop* messageLoop, const std::string& hostName, uint16_t port)
:   messageLoop_(messageLoop),
    serverAddress_()
{
    messageLoop_->postTask(std::bind(&TCPConnector::resolveInLoop, this, hostName, port));
}
    
TCPConnector::~TCPConnector()
{
    
}
    
void TCPConnector::connect()
{
    messageLoop_->postTask(std::bind(&TCPConnector::connectInLoop, this));
}
    
void TCPConnector::connect(const IPAddress& serverAddress)
{
    serverAddress_ = serverAddress;
    connect();
}

void TCPConnector::connect(const std::string& hostName, uint16_t port)
{
    messageLoop_->postTask(std::bind(&TCPConnector::resolveInLoop, this, hostName, port));
    connect();
}
    
void TCPConnector::connectInLoop()
{
    if(serverAddress_.valid()) {
        TCPSocket* socket = new TCPSocket();
        socket_ = std::shared_ptr<TCPSocket>(socket);
        socket_->open(messageLoop_);
        socket_->setConnectCallback(std::bind(&TCPConnector::didConnectComplete, this, std::placeholders::_1));
        if(socket_->connect(serverAddress_) > 0) {
            socket_->close();
            didConnectComplete(false);
        }
    } else {
        didConnectComplete(false);
    }
}
    
void TCPConnector::resolveInLoop(const std::string& hostName, uint16_t port)
{
    const std::vector<IPAddress>& addresses = IPAddress::resolve(hostName);
    if(addresses.size() > 0) {
        serverAddress_ = addresses[0];
        serverAddress_.setPort(port);
    }
}
    
void TCPConnector::didConnectComplete(bool success)
{
    if(success) {
        newTCPSessionCallback_(socket_);
    } else {
        newTCPSessionCallback_(std::shared_ptr<TCPSocket>());
    }
}
}
}