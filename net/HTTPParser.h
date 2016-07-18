//
//  HTTPParser.h
//  WukongBase
//
//  Created by Xuhui on 16/7/12.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef HTTPParser_h
#define HTTPParser_h

#include "http_parser/http_parser.h"
#include "net/URL.h"
#include <functional>
#include <string>
#include <vector>

namespace WukongBase {
    
namespace Base {
class IOBuffer;
}

namespace Net {
    
class URLResponse;
    
class HTTPParser {
public:
    typedef std::function<void()> EventCalback;
    typedef std::function<void(URLResponse&&)> ResponseCompleteCalback;
    typedef std::function<void(Base::IOBuffer&& buffer)> DataCallback;
    
    HTTPParser();
    ~HTTPParser();
    
    void parse(const char* data, size_t size);
    
    void setMessageBeginCallback(const EventCalback& cb)
    {
        messageBeginCallback_ = cb;
    }
    
    void setMessageCompleteCallback(const EventCalback& cb)
    {
        messageCompleteCallback_ = cb;
    }
    
    void setResponseCompleteCalback(const ResponseCompleteCalback& cb)
    {
        responseCompleteCalback_ = cb;
    }
    
    void setDataCallback(const DataCallback& cb)
    {
        dataCallback_ = cb;
    }
    
public:
    void onURL(const std::string& url);
    void onStatus(int statusCode, const std::string& message);
    void onMessageBegin();
    void onMessageComplete();
    void onHeaderField(const std::string& field);
    void onHeaderValue(const std::string& value);
    void onHeadersComplete();
    void onBody(Base::IOBuffer&& buffer);
    
private:
    
    struct HeaderLine {
        std::string field;
        std::string value;
    };
    
    std::vector<HeaderLine> headerLines_;
    
    int lineNum_;
    bool lastWasValue_;
    
    URL URL_;
    int statusCode_;
    std::string statusMessage_;
    
    http_parser parser_;
    http_parser_settings settings_;
    
    EventCalback messageBeginCallback_;
    EventCalback messageCompleteCallback_;
    ResponseCompleteCalback responseCompleteCalback_;
    DataCallback dataCallback_;
};
}
}

#endif /* HTTPParser_h */
