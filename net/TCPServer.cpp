//
//  TCPServer.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/8.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/TCPServer.h"

namespace WukongBase {

namespace Net {
    
TCPServer::TCPServer(Base::MessageLoop* messageLoop, const IPAddress& listenAddress)
: acceptor_(new TCPAcceptor(messageLoop, listenAddress, true))
{
    acceptor_->setNewTCPSessionCallback(std::bind(&TCPServer::didConnectComplete, this, std::placeholders::_1));
}

TCPServer::~TCPServer()
{
    
}
    
void TCPServer::start()
{
    acceptor_->listen(10);
}
    
void TCPServer::didConnectComplete(const std::shared_ptr<TCPSocket>& socket)
{
    const IPAddress& localAddress = IPAddress::getLocalAddress(*socket);
    const IPAddress& peerAddress = IPAddress::getPeerAddress(*socket);
    
    std::shared_ptr<TCPSession> session(new TCPSession(socket, localAddress, peerAddress));
    session->setReadCompleteCallback(std::bind(&TCPServer::didReadComplete, this, session, std::placeholders::_1));
    session->setWriteCompleteCallback(std::bind(&TCPServer::didWriteComplete, this, session, std::placeholders::_1));
    
    connectCallback_(session);
}

void TCPServer::didReadComplete(std::shared_ptr<TCPSession>& session, std::shared_ptr<Base::IOBuffer>& buffer)
{
    messageCallback_(session, buffer);
}

void TCPServer::didWriteComplete(std::shared_ptr<TCPSession>& session, bool success)
{
    writeCompleteCallback_(session, success);
}
    
    
}
}