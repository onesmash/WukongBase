//
//  TCPAcceptor.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/8.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/TCPAcceptor.h"


namespace WukongBase {

namespace Net {
    
TCPAcceptor::TCPAcceptor(Base::MessageLoop* messageLoop, const IPAddress& listenAddress, bool reusePort)
:   messageLoop_(messageLoop),
    socket_(new TCPSocket())
{
    socket_->open(messageLoop_);
    socket_->bind(listenAddress);
    socket_->setAcceptCallback(std::bind(&TCPAcceptor::didReciveConnectRequest, this));
    
}
    
TCPAcceptor::~TCPAcceptor()
{
}
    
void TCPAcceptor::listen(int backlog)
{
    messageLoop_->postTask(std::bind(&TCPAcceptor::listenInLoop, this, backlog));
}
    
void TCPAcceptor::didReciveConnectRequest()
{
    std::shared_ptr<TCPSocket> socket(new TCPSocket());
    socket->open(messageLoop_);
    if(socket_->accept(*socket.get()) <= 0) {
        newTCPSessionCallback_(socket);
    }
}
    
void TCPAcceptor::listenInLoop(int backlog)
{
    socket_->listen(backlog);
    socket_->startRead();
}
}
}