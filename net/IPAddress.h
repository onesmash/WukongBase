//
//  IPAddress.h
//  AppCore
//
//  Created by Xuhui on 15/5/26.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#ifndef __Net__IPAddress__
#define __Net__IPAddress__

#include <string>
#include <vector>
#include <functional>
#include <netinet/in.h>
#include <arpa/inet.h>


namespace WukongBase {
    
namespace Base {
    class MessageLoop;
}
    
namespace Net {
    
class TCPSocket;
    
class IPAddress {
    
public:
    IPAddress();
    IPAddress(uint16_t port, bool loopback = false, bool isIPv6 = false);
    IPAddress(const std::string& ip, uint16_t port, bool isIPv6 = false);
    IPAddress(const sockaddr* address);
    IPAddress(const IPAddress& address);
    virtual ~IPAddress();
    
    void setPort(uint16_t port) { address_.sin_port = htons(port); }
    
    bool valid() { return valid_; }
    
    const sockaddr* sockAddress() const;
    
    bool operator==(const IPAddress& address)
    {
        if(!valid_ || address.valid_)
            return false;
        if(address_.sin_family != address.address_.sin_family)
            return false;
        if(address_.sin_port != address.address_.sin_port)
            return false;
        if(address_.sin_family == AF_INET) {
            return memcmp((const void*)&address_.sin_addr, (const void*)&address.address_.sin_addr, sizeof(address_.sin_addr));
        } else if(address6_.sin6_family == AF_INET6) {
            return memcmp((const void*)&address6_.sin6_addr, (const void*)&address.address6_.sin6_addr, sizeof(address6_.sin6_addr));
        }
        return false;
    }
    
    bool isIPv6() const
    {
        return address6_.sin6_family == AF_INET6;
    }
    
    static std::vector<IPAddress> resolve(const std::string& hostName, bool isTCP = true);
    
private:
    
    bool valid_;
    
    union {
        sockaddr_in address_;
        sockaddr_in6 address6_;
    };
    
    
};
    
}
    
}

#endif /* defined(__Net__IPAddress__) */
