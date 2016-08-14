//
//  URLRequest.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/11.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "URLRequest.h"
#include <sstream>

namespace WukongBase {

namespace Net {
    
URLRequest::URLRequest(const std::string& URLString): URL_(URLString), version_("1.1")
{
}
    
URLRequest::URLRequest(const URL& url): URL_(url), version_("1.1")
{
}
    
URLRequest::URLRequest(const URLRequest& request)
:   URL_(request.URL_),
    version_(request.version_),
    method_(request.method_),
    body_(request.body_),
    headers_(request.headers_)
{
}
    
URLRequest::URLRequest(URLRequest&& request)
:   URL_(std::move(request.URL_)),
    version_(std::move(request.version_)),
    method_(std::move(request.method_)),
    body_(std::move(request.body_)),
    headers_(std::move(request.headers_))
{
}
    
URLRequest::~URLRequest()
{
}
    
void URLRequest::setURL(const std::string& URLString)
{
    URL_ = URL(URLString);
}

void URLRequest::setHTTPMethod(const std::string& method)
{
    method_ = method;
}

void URLRequest::setHTTPBody(const std::vector<char>& body)
{
    body_ = body;
}

void URLRequest::setHTTPBody(std::vector<char>&& body)
{
    body_ = std::move(body);
}

void URLRequest::addHeader(const std::string& field, const std::string& value)
{
    headers_[field] = value;
}
    
Packet URLRequest::pack() const
{
    Packet packet;
    std::stringstream data;
    data << method_ << " " << URL_.path() << " HTTP/" << version_ << "\r\n";
    data << "Host: " << URL_.host() << ":" << URL_.port() << "\r\n";
    for (std::unordered_map<std::string, std::string>::const_iterator iter = headers_.begin(); iter != headers_.end(); ++iter) {
        data << iter->first << ": " << iter->second << "\r\n";
    }
    data << "\r\n";
    data.write(&*body_.begin(), body_.size());
    packet.append((void*)data.str().c_str(), data.str().size());
    return packet;
}

bool URLRequest::unpack(Packet& packet)
{
    return false;
}
}
}