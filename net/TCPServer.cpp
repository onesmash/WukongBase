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
: acceptor_(new TCPAcceptor(messageLoop, listenAddress, true, threadNum))
{
    acceptor_->setNewTCPSessionCallback(std::bind(&TCPServer::didConnectComplete, this, std::placeholders::_1));
    acceptor_->setStopCallback(stopCallback_);
}

TCPServer::~TCPServer()
{
    
}
    
void TCPServer::start()
{
    acceptor_->listen(10);
}
    
void TCPServer::stop()
{
    acceptor_->stop();
}
    
void TCPServer::didConnectComplete(const std::shared_ptr<TCPSocket>& socket)
{
    const IPAddress& localAddress = socket->getLocalAddress();
    const IPAddress& peerAddress = socket->getPeerAddress();
    
    std::shared_ptr<TCPSession> session(new TCPSession(socket, localAddress, peerAddress));
    session->setReadCompleteCallback(std::bind(&TCPServer::didReadComplete, this, session, std::placeholders::_1));
    session->setWriteCompleteCallback(std::bind(&TCPServer::didWriteComplete, this, session, std::placeholders::_1, std::placeholders::_2));
    
    connectCallback_(session);
}

void TCPServer::didReadComplete(const std::shared_ptr<TCPSession>& session, std::shared_ptr<Packet>& buffer)
{
    messageCallback_(session, buffer);
}

void TCPServer::didWriteComplete(const std::shared_ptr<TCPSession>& session, const Packet& packet, bool success)
{
    writeCompleteCallback_(session, packet, success);
}
    
    
}
}