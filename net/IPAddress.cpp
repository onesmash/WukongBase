//
//  IPAddress.cpp
//  AppCore
//
//  Created by Xuhui on 15/5/26.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#include "net/IPAddress.h"
#include "net/TCPSocket.h"
#include "libuv/include/uv.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdlib>

namespace WukongBase {
    
namespace Net {
    
void onGetaddrinfoComplete(uv_getaddrinfo_t* req, int status, struct addrinfo* res)
{
    
}
    
IPAddress::IPAddress(): valid_(false)
{
}
    
IPAddress::IPAddress(uint16_t port, bool loopback, bool isIPv6): valid_(true)
{
    if(isIPv6) {
        bzero(&address6_, sizeof address6_);
        address6_.sin6_family = AF_INET6;
        in6_addr ip = loopback ? in6addr_loopback : in6addr_any;
        address6_.sin6_addr = ip;
        address6_.sin6_port = htons(port);
    } else {
        bzero(&address_, sizeof address_);
        address_.sin_family = AF_INET;
        in_addr_t ip = loopback ? INADDR_LOOPBACK : INADDR_ANY;
        address_.sin_addr.s_addr = ntohl(ip);
        address_.sin_port = htons(port);
    }
}
    
IPAddress::IPAddress(const sockaddr* address): valid_(true)
{
    address6_ = *(const sockaddr_in6*)address;
}
    
IPAddress::IPAddress(const std::string& ip, uint16_t port, bool isIPv6): valid_(true)
{
    if(isIPv6) {
        uv_ip6_addr(ip.c_str(), port, &address6_);
    } else {
        uv_ip4_addr(ip.c_str(), port, &address_);
    }
}
    
IPAddress::IPAddress(const IPAddress& address): valid_(address.valid_)
{
    address_ = address.address_;
}
    
IPAddress::~IPAddress()
{
    
}
    
const sockaddr* IPAddress::sockAddress() const
{
    return reinterpret_cast<const struct sockaddr*>(&address6_);
}
    
std::vector<IPAddress> IPAddress::resolve(const std::string& hostName, bool isTCP)
{
    std::vector<IPAddress> addresses;
    addrinfo hints;
    addrinfo* info = NULL;
    
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = isTCP ? SOCK_STREAM : SOCK_DGRAM;
    int err = getaddrinfo(hostName.c_str(), NULL, NULL, &info);
    if(err) {
        return addresses;
    }
    for (addrinfo* res = info; res; res = res->ai_next) {
        addresses.push_back(IPAddress(res->ai_addr));
    }
    return addresses;
}
    
}
    
}