//
//  URLResponse.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/12.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/URLResponse.h"

namespace WukongBase {
    
namespace Net {
URLResponse::URLResponse(const URL& url, int statusCode)
:   URL_(url),
    statusCode_(statusCode)
{
    
}

URLResponse::URLResponse(const URLResponse& response)
:   URL_(response.URL_),
    statusCode_(response.statusCode_),
    headers_(response.headers_)
{
    
}

URLResponse::URLResponse(URLResponse&& response)
:   URL_(std::move(response.URL_)),
    statusCode_(response.statusCode_),
    headers_(std::move(response.headers_))
{
    
}
    
URLResponse::~URLResponse()
{
    
}

URLResponse& URLResponse::operator=(const URLResponse& response)
{
    URL_ = response.URL_;
    statusCode_ = response.statusCode_;
    headers_ = response.headers_;
    return *this;
}

URLResponse& URLResponse::operator=(URLResponse&& response)
{
    URL_ = std::move(response.URL_);
    statusCode_ = response.statusCode_;
    headers_ = std::move(response.headers_);
    return *this;
}
    
void URLResponse::addHeader(const std::string& field, const std::string& value)
{
    headers_.insert({field, value});
}
    
std::string URLResponse::getHeaderValue(const std::string& field)
{
    return headers_[field];
}
    
}
}