//
//  HTTPSession.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/11.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/HTTPSession.h"
#include "net/TCPSession.h"
#include "net/TCPClient.h"
#include "net/URLResponse.h"

namespace WukongBase {

namespace Net {
    
HTTPSession::HTTPSession(const std::shared_ptr<TCPClient>& tcpClient)
:   state_(kDisconnected),
    tcpClient_(tcpClient)
{
    tcpClient_->setConnectCallback(std::bind(&HTTPSession::didConnectComplete, this, std::placeholders::_1));
    tcpClient_->setMessageCallback(std::bind(&HTTPSession::didRecvMessageComplete, this, std::placeholders::_1, std::placeholders::_2));
    tcpClient_->setWriteCompleteCallback(std::bind(&HTTPSession::didWriteComplete, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    tcpClient_->setCloseCallback(std::bind(&HTTPSession::didCloseComplete, this));
    parser_.setDataCallback(std::bind(&HTTPSession::didRecvData, this, std::placeholders::_1));
    parser_.setMessageBeginCallback(std::bind(&HTTPSession::didBeginParse, this));
    parser_.setMessageCompleteCallback(std::bind(&HTTPSession::didCompleteParse, this));
    parser_.setResponseCompleteCalback(std::bind(&HTTPSession::didRecvResponse, this, std::placeholders::_1));
}
    
HTTPSession::~HTTPSession()
{
}
    
void HTTPSession::sendRequest(const std::shared_ptr<URLRequest>& request)
{
    if(state_ == kDisconnected) {
        {
            std::lock_guard<std::mutex> guard(lock_);
            state_ = kConnecting;
        }
        tcpClient_->connect();
    } else {
        sendRequestInternal(request);
    }
    std::lock_guard<std::mutex> guard(lock_);
    paddingRequests_.push_back(request);
}
    
void HTTPSession::close()
{
    {
        std::lock_guard<std::mutex> guard(lock_);
        state_ = kConnecting;
    }
    tcpClient_->disconnect();
}
    
void HTTPSession::sendRequestInternal(const std::shared_ptr<URLRequest>& request)
{
    tcpSession_->send(request->pack());
}
    
void HTTPSession::didConnectComplete(const std::shared_ptr<TCPSession>& session)
{
    {
        std::lock_guard<std::mutex> guard(lock_);
        state_ = kConnected;
    }
    
    tcpSession_ = session;
    tcpSession_->startRead();
    std::lock_guard<std::mutex> guard(lock_);
    for (std::list<std::shared_ptr<URLRequest>>::const_iterator iter = paddingRequests_.begin(); iter != paddingRequests_.end(); ++iter) {
        sendRequestInternal(*iter);
    }
}
    
void HTTPSession::didCloseComplete()
{
    {
        std::lock_guard<std::mutex> guard(lock_);
        state_ = kConnected;
    }
    closeCallback_(*this);
}

void HTTPSession::didRecvMessageComplete(const std::shared_ptr<TCPSession>& session, const std::shared_ptr<Packet>& buffer)
{
    parser_.parse(buffer->data(), buffer->size());
}

void HTTPSession::didWriteComplete(const std::shared_ptr<TCPSession>& session, const Packet& packet, bool success)
{
    requestCallback_(*this, paddingRequests_.front(), success);
}
    
void HTTPSession::didBeginParse()
{
    
}

void HTTPSession::didCompleteParse()
{
    dataCompleteCallback_(*this, paddingRequests_.front());
    std::lock_guard<std::mutex> guard(lock_);
    paddingRequests_.pop_back();
}

void HTTPSession::didRecvResponse(URLResponse&& response)
{
    responseCallback_(*this, paddingRequests_.front(), std::move(response));
}

void HTTPSession::didRecvData(Packet&& buffer)
{
    dataCallback_(*this, paddingRequests_.front(), std::move(buffer));
}
    
    
}
}
