//
//  TCPServer.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/8.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/TCPServer.h"
#include "net/Packet.h"

namespace WukongBase {

namespace Net {
    
TCPServer::TCPServer(Base::MessageLoop* messageLoop, const IPAddress& listenAddress, int threadNum)
: acceptor_(new TCPAcceptor(messageLoop, listenAddress, true, threadNum)), isStarted_(false)
{
    acceptor_->setNewTCPSessionCallback(std::bind(&TCPServer::didConnectComplete, this, std::placeholders::_1));
    acceptor_->setStopCallback([this]() {
        stopCallback_();
    });
}

TCPServer::~TCPServer()
{
}
    
void TCPServer::start()
{
    acceptor_->listen(10);
    isStarted_ = true;
}
    
void TCPServer::stop()
{
    acceptor_->stop();
    isStarted_ = false;
}
    
void TCPServer::didConnectComplete(const std::shared_ptr<TCPSocket>& socket)
{
    const IPAddress& localAddress = socket->getLocalAddress();
    const IPAddress& peerAddress = socket->getPeerAddress();
    
    std::shared_ptr<TCPSession> session(new TCPSession(socket, localAddress, peerAddress));
    connectCallback_(session);
}    
    
}
}
