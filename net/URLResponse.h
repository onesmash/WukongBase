//
//  URLResponse.h
//  WukongBase
//
//  Created by Xuhui on 16/7/12.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef URLResponse_h
#define URLResponse_h

#include "net/URL.h"
#include <unordered_map>
#include <string>

namespace WukongBase {

namespace Net {
    
class URLResponse {
public:
    URLResponse(const URL& url, int statusCode);
    URLResponse(const URLResponse& response);
    URLResponse(URLResponse&& response);
    ~URLResponse();
    
    URLResponse& operator=(const URLResponse& response);
    URLResponse& operator=(URLResponse&& response);
    
    void addHeader(const std::string& field, const std::string& value);
    
    std::string getHeaderValue(const std::string& field);
    
    void setContentLength(size_t length) { contenLength_ = length; }
    size_t getContentLength() { return contenLength_; }
    int getStatusCode() { return statusCode_; }
    
    const std::unordered_map<std::string, std::string>& getAllHeader() { return  headers_; }
    
private:
    URL URL_;
    int statusCode_;
    size_t contenLength_;
    std::unordered_map<std::string, std::string> headers_;
};
}
}

#endif /* URLResponse_h */
