//
//  URL.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/13.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "URL.h"
#include "http_parser/http_parser.h"


namespace WukongBase {

namespace Net {
    
URL::URL(const std::string& URLString): URLString_(URLString), scheme_("http"), port_(80), path_("/")
{
    http_parser_url urlInfo;
    http_parser_url_init(&urlInfo);
    http_parser_parse_url(URLString_.c_str(), URLString_.size(), false, &urlInfo);
    for (int field = UF_SCHEMA; field < UF_MAX; field++) {
        if((urlInfo.field_set & (1 << field)) <= 0) continue;
        switch (field) {
            case UF_SCHEMA: {
                scheme_ = std::string(URLString_.c_str() + urlInfo.field_data[field].off, urlInfo.field_data[field].len);
            } break;
            case UF_USERINFO: {
                user_ = std::string(std::strtok((char*)URLString_.c_str() + urlInfo.field_data[field].off, ":@"));
                password_ = std::string(std::strtok(NULL, ":@"));
            } break;
            case UF_HOST: {
                host_ = std::string(URLString_.c_str() + urlInfo.field_data[field].off, urlInfo.field_data[field].len);
            } break;
            case UF_PORT: {
                port_ = urlInfo.port;
            } break;
            case UF_PATH: {
                path_ = std::string(URLString_.c_str() + urlInfo.field_data[field].off, urlInfo.field_data[field].len);
            } break;
            case UF_FRAGMENT: {
                fragment_ = std::string(URLString_.c_str() + urlInfo.field_data[field].off, urlInfo.field_data[field].len);
            } break;
            case UF_QUERY: {
                query_ = std::string(URLString_.c_str() + urlInfo.field_data[field].off, urlInfo.field_data[field].len);
            } break;
            default:
                break;
        }
    }
}
    
URL::URL(const URL& url)
:   URLString_(url.URLString_),
    scheme_(url.scheme_),
    user_(url.user_),
    password_(url.password_),
    host_(url.host_),
    port_(url.port_),
    path_(url.path_),
    fragment_(url.fragment_),
    query_(url.query_)
{
}
}
}