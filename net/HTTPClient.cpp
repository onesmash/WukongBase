//
//  HTTPClient.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/11.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/HTTPClient.h"
#include "net/TCPClient.h"
#include "net/URLRequest.h"
#include "net/HTTPSession.H"
#include "net/URLResponse.h"

namespace WukongBase {
    
namespace Net {
    
HTTPClient::HTTPClient(int threadNum)
:   threadNum_(threadNum),
    threadPool_(threadNum_)
{
    threadPool_.start();
}
    
HTTPClient::~HTTPClient()
{
    threadPool_.stop();
}
    
void HTTPClient::executeRequest(const std::shared_ptr<URLRequest>& request)
{
    if(request->getHeader("User-Agent").size() <= 0) {
        request->addHeader("User-Agent", userAgent_);
    }
    const std::shared_ptr<Base::Thread>& thread = threadPool_.getThread();
    const URL& url = request->getURL();
    std::shared_ptr<TCPClient> client(new TCPClient(thread->messageLoop(), url.host(), url.port()));
    std::shared_ptr<HTTPSession> session(new HTTPSession(client));
    session->setRequestCallback(std::bind(&HTTPClient::didSendRequestComplete, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    session->setResponseCallback(std::bind(&HTTPClient::didRecvResponseComplete, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, responseCallback_));
    session->setDataCallback(std::bind(&HTTPClient::didRecvData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    session->setDataCompleteCallback(std::bind(&HTTPClient::didRecvDataComplete, this, std::placeholders::_1, std::placeholders::_2));
    session->setCloseCallback(std::bind(&HTTPClient::didCloseSessionComplete, this, std::placeholders::_1));
    session->sendRequest(request);
    std::lock_guard<std::mutex> guard(lock_);
    sessionMap_[session.get()] = session;
}
    
void HTTPClient::executeRequest(const std::shared_ptr<URLRequest>& request, const RequestCallback& requestCallback, const ResponseCallback& responseCallback, const DataCallback& dataCallback, const DataCompleteCallback& dataCompleteCallback)
{
    if(request->getHeader("User-Agent").size() <= 0) {
        request->addHeader("User-Agent", userAgent_);
    }
    const std::shared_ptr<Base::Thread>& thread = threadPool_.getThread();
    const URL& url = request->getURL();
    std::shared_ptr<TCPClient> client(new TCPClient(thread->messageLoop(), url.host(), url.port()));
    std::shared_ptr<HTTPSession> session(new HTTPSession(client));
    session->setRequestCallback(std::bind(requestCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    session->setResponseCallback(std::bind(&HTTPClient::didRecvResponseComplete, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, responseCallback));
    session->setDataCallback(std::bind(dataCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    session->setDataCompleteCallback(std::bind(dataCompleteCallback, std::placeholders::_1, std::placeholders::_2));
    session->setCloseCallback(std::bind(&HTTPClient::didCloseSessionComplete, this, std::placeholders::_1));
    session->sendRequest(request);
    std::lock_guard<std::mutex> guard(lock_);
    sessionMap_[session.get()] = session;
}
    
void HTTPClient::didSendRequestComplete(const HTTPSession& session, const std::shared_ptr<URLRequest>& request, bool success)
{
    requestCallback_(session, request, success);
}

void HTTPClient::didRecvResponseComplete(const HTTPSession& session, const std::shared_ptr<URLRequest>& request, URLResponse&& response, const ResponseCallback& responseCallback)
{
    if(response.getStatusCode() == 302) {
        const std::string& location = response.getHeaderValue("Location");
        request->setURL(location);
        executeRequest(request, session.getRequestCallback(), session.getResponseCallback(), session.getDataCallback(), session.getDataCompleteCallback());
    } else {
        responseCallback(session, request, std::move(response));
    }
}

void HTTPClient::didRecvData(const HTTPSession& session, const std::shared_ptr<URLRequest>& request, Packet&& buffer)
{
    dataCallback_(session, request, std::move(buffer));
}
    
void HTTPClient::didRecvDataComplete(const HTTPSession& session, const std::shared_ptr<URLRequest>& request)
{
    dataCompleteCallback_(session, request);
}

void HTTPClient::didCloseSessionComplete(const HTTPSession& session)
{
    std::lock_guard<std::mutex> guard(lock_);
    sessionMap_.erase((HTTPSession*)&session);
}
    
}
}