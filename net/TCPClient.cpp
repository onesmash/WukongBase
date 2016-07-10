//
//  TCPClient.cpp
//  Wukong
//
//  Created by Xuhui on 16/7/4.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/TCPClient.h"
#include "base/message_loop/MessageLoop.h"
#include "net/TCPConnector.h"

namespace WukongBase {

namespace Net {

TCPClient::TCPClient(Base::MessageLoop* messageLoop, const IPAddress& serverAddress)
    :   connector_(new TCPConnector(messageLoop, serverAddress))
{
    connector_->setNewTCPSessionCallback(std::bind(&TCPClient::didConnectComplete, this, std::placeholders::_1));
}
  
TCPClient::~TCPClient()
{
}
    
void TCPClient::connect()
{
    connector_->connect();
}

void TCPClient::disconnect()
{
    session_->shutdown();
}
    
void TCPClient::didConnectComplete(const std::shared_ptr<TCPSocket>& socket)
{
    if(socket) {
        const IPAddress& localAddress = IPAddress::getLocalAddress(*socket);
        const IPAddress& peerAddress = IPAddress::getPeerAddress(*socket);
        
        session_ =  std::shared_ptr<TCPSession>(new TCPSession(socket, localAddress, peerAddress));
        session_->setReadCompleteCallback(std::bind(&TCPClient::didReadComplete, this, std::placeholders::_1));
        session_->setWriteCompleteCallback(std::bind(&TCPClient::didWriteComplete, this, std::placeholders::_1));
        session_->setCloseCallback(std::bind(&TCPClient::didCloseComplete, this));
        
        connectCallback_(session_);
    }
}
    
void TCPClient::didReadComplete(std::shared_ptr<Base::IOBuffer>& buffer)
{
    messageCallback_(session_, buffer);
}

void TCPClient::didWriteComplete(bool success)
{
    writeCompleteCallback_(session_, success);
}

void TCPClient::didCloseComplete()
{
}
    
}
}