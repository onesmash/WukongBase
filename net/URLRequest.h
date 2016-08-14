//
//  URLRequest.h
//  WukongBase
//
//  Created by Xuhui on 16/7/11.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#ifndef URLRequest_h
#define URLRequest_h

#include "net/URL.h"
#include "net/Packer.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace WukongBase {

namespace Net {
    
class URLRequest: public Packer {
public:
    URLRequest(const std::string& URLString);
    URLRequest(const URL& url);
    URLRequest(const URLRequest& request);
    URLRequest(URLRequest&& request);
    ~URLRequest();
    
    URLRequest& operator=(const URLRequest& request);
    URLRequest& operator=(URLRequest&& request);
    
    void setURL(const std::string& URLString);
    const URL& getURL() const { return URL_; }
    
    void setVersion(const std::string& version) { version_ = version; }
    void setHTTPMethod(const std::string& method);
    void setHTTPBody(const std::vector<char>& body);
    void setHTTPBody(std::vector<char>&& data);
    void addHeader(const std::string& field, const std::string& value);
    std::string getHeader(const std::string& field) { return headers_[field]; }
    
    virtual Packet pack() const;
    virtual bool unpack(Packet& packet);
    
private:
    URL URL_;
    std::string version_;
    std::string method_;
    std::vector<char> body_;
    std::unordered_map<std::string, std::string> headers_;
};

}
}

#endif /* URLRequest_h */
