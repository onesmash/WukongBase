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
#include "net/Packet.h"

namespace WukongBase {

namespace Net {

TCPClient::TCPClient(Base::MessageLoop* messageLoop, const IPAddress& serverAddress)
    :   connector_(new TCPConnector(messageLoop, serverAddress))
{
    connector_->setNewTCPSessionCallback(std::bind(&TCPClient::didConnectComplete, this, std::placeholders::_1));
}
    
TCPClient::TCPClient(Base::MessageLoop* messageLoop, const std::string& hostName, uint16_t port)
:   connector_(new TCPConnector(messageLoop, hostName, port))
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
    
void TCPClient::connect(const IPAddress& serverAddress)
{
    connector_->connect(serverAddress);
}

void TCPClient::connect(const std::string& hostName, uint16_t port)
{
    connector_->connect(hostName, port);
}

void TCPClient::disconnect()
{
    session_->shutdown();
}
    
void TCPClient::didConnectComplete(const std::shared_ptr<TCPSocket>& socket)
{
    if(socket) {
        const IPAddress& localAddress = socket->getLocalAddress();
        const IPAddress& peerAddress = socket->getPeerAddress();
        
        session_ =  std::shared_ptr<TCPSession>(new TCPSession(socket, localAddress, peerAddress));
        session_->setReadCompleteCallback(std::bind(&TCPClient::didReadComplete, this, std::placeholders::_1));
        session_->setWriteCompleteCallback(std::bind(&TCPClient::didWriteComplete, this, std::placeholders::_1, std::placeholders::_2));
        session_->setCloseCallback(std::bind(&TCPClient::didCloseComplete, this));
        
        connectCallback_(session_);
    }
}
    
void TCPClient::didReadComplete(std::shared_ptr<Packet>& buffer)
{
    messageCallback_(session_, buffer);
}

void TCPClient::didWriteComplete(const Packet& packet, bool success)
{
    writeCompleteCallback_(session_, packet, success);
}

void TCPClient::didCloseComplete()
{
    closeCallback_();
}
    
}
}