//
//  HTTPSession.h
//  WukongBase
//
//  Created by Xuhui on 16/7/11.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef HTTPSession_h
#define HTTPSession_h

#include "net/URLRequest.h"
#include "net/HTTPParser.h"
#include <list>

namespace WukongBase {
    
namespace Net {
    
class TCPClient;
class TCPSession;
class URLResponse;
class Packet;
    
class HTTPSession {

public:
    typedef std::function<void(const HTTPSession&, const std::shared_ptr<URLRequest>&, bool)> RequestCallback;
    typedef std::function<void(const HTTPSession&, const std::shared_ptr<URLRequest>&, URLResponse&&)> ResponseCallback;
    typedef std::function<void(const HTTPSession&, const std::shared_ptr<URLRequest>&, Packet&&)> DataCallback;
    typedef std::function<void(const HTTPSession&, const std::shared_ptr<URLRequest>&)> DataCompleteCallback;
    typedef std::function<void(const HTTPSession&)> CloseCallback;
    HTTPSession(const std::shared_ptr<TCPClient>& tcpClient);
    ~HTTPSession();
    
    void sendRequest(const std::shared_ptr<URLRequest>& request);
    
    void close();
    
    void setRequestCallback(const RequestCallback& cb)
    {
        requestCallback_ = cb;
    }
    
    void setResponseCallback(const ResponseCallback& cb)
    {
        responseCallback_ = cb;
    }
    
    void setDataCallback(const DataCallback& cb)
    {
        dataCallback_ = cb;
    }
    
    void setDataCompleteCallback(const DataCompleteCallback& cb)
    {
        dataCompleteCallback_ = cb;
    }
    
    void setCloseCallback(const CloseCallback& cb)
    {
        closeCallback_ = cb;
    }
    
    const RequestCallback& getRequestCallback() const
    {
        return requestCallback_;
    }
    
    const ResponseCallback& getResponseCallback() const
    {
        return responseCallback_;
    }
    
    const DataCallback& getDataCallback() const
    {
        return dataCallback_;
    }
    
    const DataCompleteCallback& getDataCompleteCallback() const
    {
        return dataCompleteCallback_;
    }
    
    const CloseCallback& getCloseCallback() const
    {
        return closeCallback_;
    }
    
private:
    
    void sendRequestInternal(const std::shared_ptr<URLRequest>& request);
    
    void didConnectComplete(const std::shared_ptr<TCPSession>&);
    void didCloseComplete();
    void didRecvMessageComplete(const std::shared_ptr<TCPSession>&, const std::shared_ptr<Packet>&);
    void didWriteComplete(const std::shared_ptr<TCPSession>&, const Packet&, bool);
    
    void didBeginParse();
    void didCompleteParse();
    void didRecvResponse(URLResponse&& response);
    void didRecvData(Packet&& buffer);
    
    enum State { kDisconnected, kConnecting, kConnected, kDisconnecting };
    
    State state_;
    
    std::list<std::shared_ptr<URLRequest>> paddingRequests_;
    std::shared_ptr<TCPClient> tcpClient_;
    std::shared_ptr<TCPSession> tcpSession_;
    HTTPParser parser_;
    
    RequestCallback requestCallback_;
    ResponseCallback responseCallback_;
    DataCallback dataCallback_;
    DataCompleteCallback dataCompleteCallback_;
    CloseCallback closeCallback_;
    
    std::string host_;
    
};
}
}

#endif /* HTTPSession_h */
