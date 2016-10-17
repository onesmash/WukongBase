//
//  TCPAcceptor.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/8.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/TCPAcceptor.h"
#include "base/message_loop/MessageLoop.h"
#include <cassert>

namespace WukongBase {

namespace Net {
    
TCPAcceptor::TCPAcceptor(Base::MessageLoop* messageLoop, const IPAddress& listenAddress, bool reusePort, int threadNum)
:   messageLoop_(messageLoop),
    socket_(new TCPSocket()),
    threadNum_(threadNum),
    threadPool_(threadNum_)
{
    socket_->open(messageLoop_);
    socket_->bind(listenAddress);
    socket_->setAcceptCallback(std::bind(&TCPAcceptor::didReciveConnectRequest, this));
    socket_->setCloseCallback([this](bool) {
        stopCallback_();
    });
    //threadPool_.start();
}
    
TCPAcceptor::~TCPAcceptor()
{
    if(!socket_->isClosed()) {
        std::shared_ptr<TCPSocket> socket = socket_;
        messageLoop_->postTask([socket]() {
            socket->kill();
        });
    }
    threadPool_.stop();
}
    
void TCPAcceptor::listen(int backlog)
{
    messageLoop_->postTask(std::bind(&TCPAcceptor::listenInLoop, this, backlog));
}
    
void TCPAcceptor::stop()
{
    messageLoop_->postTask([this]() {
        socket_->close();
    });
}
    
void TCPAcceptor::didReciveConnectRequest()
{
    //const std::shared_ptr<Base::Thread>& thread = threadPool_.getThread();
    std::shared_ptr<TCPSocket> socket(new TCPSocket());
    socket->open(messageLoop_);
    if(socket_->accept(*socket.get()) <= 0) {
        newTCPSessionCallback_(socket);
    }
}
    
void TCPAcceptor::listenInLoop(int backlog)
{
    assert(socket_->listen(backlog) == 0);
    socket_->startRead();
}
}
}
