//
//  URL.h
//  WukongBase
//
//  Created by Xuhui on 16/7/13.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef URL_h
#define URL_h

#include <string>

namespace WukongBase {

namespace Net {
    
class URL {
public:
    URL() {}
    URL(const std::string& URLString);
    URL(const URL& url);
    ~URL() {}
    
    const std::string& scheme() const { return scheme_; }
    const std::string& user() const { return user_; }
    const std::string& password() const { return password_; }
    const std::string& host() const { return host_; }
    const uint16_t& port() const { return port_; }
    const std::string& path() const { return path_; }
    const std::string& fragment() const { return fragment_; }
    const std::string& query() const { return query_; }
    
private:
    std::string URLString_;
    std::string scheme_;
    std::string user_;
    std::string password_;
    std::string host_;
    uint16_t port_;
    std::string path_;
    std::string fragment_;
    std::string query_;
    
};
}
}

#endif /* URL_h */
