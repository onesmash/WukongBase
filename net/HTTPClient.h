//
//  HTTPClient.h
//  WukongBase
//
//  Created by Xuhui on 16/7/11.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef HTTPClient_h
#define HTTPClient_h

#include "base/thread/ThreadPool.h"
#include <string>
#include <unordered_map>

namespace WukongBase {
    
namespace Base {
class MessageLoop;
class IOBuffer;
}

namespace Net {

class URLRequest;
class HTTPSession;
class URLResponse;
    
class HTTPClient {
public:
    typedef std::function<void(const HTTPSession&, const std::shared_ptr<URLRequest>&, bool)> RequestCallback;
    typedef std::function<void(const HTTPSession&, const std::shared_ptr<URLRequest>&, URLResponse&&)> ResponseCallback;
    typedef std::function<void(const HTTPSession&, const std::shared_ptr<URLRequest>&, Base::IOBuffer&&)> DataCallback;
    typedef std::function<void(const HTTPSession&, const std::shared_ptr<URLRequest>&)> DataCompleteCallback;
    
    HTTPClient(int threadNum);
    ~HTTPClient();
    
    void setUserAgent(const std::string& userAgent) { userAgent_ = userAgent; }
    void executeRequest(const std::shared_ptr<URLRequest>& request);
    void executeRequest(const std::shared_ptr<URLRequest>& request, const RequestCallback&, const ResponseCallback&, const DataCallback&, const DataCompleteCallback& cb);
    
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
    
private:
    
    void didSendRequestComplete(const HTTPSession&, const std::shared_ptr<URLRequest>&, bool);
    void didRecvResponseComplete(const HTTPSession&, const std::shared_ptr<URLRequest>&, URLResponse&&, const ResponseCallback& responseCallback);
    void didRecvData(const HTTPSession&, const std::shared_ptr<URLRequest>&, Base::IOBuffer&&);
    void didRecvDataComplete(const HTTPSession&, const std::shared_ptr<URLRequest>&);
    void didCloseSessionComplete(const HTTPSession&);
    
    int threadNum_;
    Base::ThreadPool threadPool_;
    std::string userAgent_;
    
    RequestCallback requestCallback_;
    ResponseCallback responseCallback_;
    DataCallback dataCallback_;
    DataCompleteCallback dataCompleteCallback_;
    
    std::unordered_map<HTTPSession*, std::shared_ptr<HTTPSession>> sessionMap_;
};
}
}


#endif /* HTTPClient_h */
