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
: messageLoop_(messageLoop), listenAddress_(listenAddress), threadNum_(threadNum), isStarted_(false)
{
    
}

TCPServer::~TCPServer()
{
}
    
void TCPServer::start()
{
    acceptor_ = std::shared_ptr<TCPAcceptor>(new TCPAcceptor(messageLoop_, listenAddress_, true, threadNum_));
    acceptor_->setNewTCPSessionCallback(std::bind(&TCPServer::didConnectComplete, this, std::placeholders::_1));
    acceptor_->setStopCallback([this]() {
        stopCallback_();
    });
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
    TCPSession* key = session.get();
    session->setDefaultCloseCallback([key](bool) {
        TCPServer::lock_.lock();
        TCPServer::sessions_.erase(key);
        TCPServer::lock_.unlock();
    });
    TCPServer::lock_.lock();
    sessions_.insert({session.get(), session});
    TCPServer::lock_.unlock();
    connectCallback_(session);
}

std::mutex TCPServer::lock_;
    
std::unordered_map<TCPSession*, std::shared_ptr<TCPSession>> TCPServer::sessions_;
    
}
}
