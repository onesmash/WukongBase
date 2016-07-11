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
