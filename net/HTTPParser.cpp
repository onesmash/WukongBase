//
//  HTTPParser.cpp
//  WukongBase
//
//  Created by Xuhui on 16/7/12.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/HTTPParser.h"
#include "net/Packet.h"
#include "net/URLResponse.h"
#include <cstdlib>
#include <cassert>
#include <iostream>

namespace WukongBase {

namespace Net {
    
int onMessageBegin(http_parser* parser)
{
    HTTPParser* p = (HTTPParser*)parser->data;
    p->onMessageBegin();
    return 0;
}
    
int onMessageComplete(http_parser* parser)
{
    HTTPParser* p = (HTTPParser*)parser->data;
    p->onMessageComplete();
    return 0;
}

int onStatus(http_parser* parser, const char *at, size_t length)
{
    HTTPParser* p = (HTTPParser*)parser->data;
    std::string message(at, length);
    p->onStatus(parser->status_code, message);
    return 0;
}

int onURL(http_parser* parser, const char *at, size_t length)
{
    HTTPParser* p = (HTTPParser*)parser->data;
    std::string URL(at, length);
    p->onURL(URL);
    return 0;
}
    
int onHeaderField(http_parser* parser, const char *at, size_t length)
{
    HTTPParser* p = (HTTPParser*)parser->data;
    std::string field(at, length);
    p->onHeaderField(field);
    return 0;
}

int onHeaderValue(http_parser* parser, const char *at, size_t length)
{
    HTTPParser* p = (HTTPParser*)parser->data;
    std::string value(at, length);
    p->onHeaderValue(value);
    return 0;
}

int onHeaderComplete(http_parser* parser)
{
    HTTPParser* p = (HTTPParser*)parser->data;
    p->onHeadersComplete();
    return 0;
}

int onBody(http_parser* parser, const char *at, size_t length)
{
    HTTPParser* p = (HTTPParser*)parser->data;
    Packet packet;
    packet.append((void*)at, length);
    p->onBody(std::move(packet));
    return 0;
}
    
HTTPParser::HTTPParser()
:   headerLines_(50),
    lineNum_(0),
    lastWasValue_(true)
{
    http_parser_init(&parser_, HTTP_BOTH);
    http_parser_settings_init(&settings_);
    settings_.on_url = Net::onURL;
    settings_.on_body = Net::onBody;
    settings_.on_status = Net::onStatus;
    settings_.on_header_field = Net::onHeaderField;
    settings_.on_header_value = Net::onHeaderValue;
    settings_.on_headers_complete = Net::onHeaderComplete;
    settings_.on_message_begin = Net::onMessageBegin;
    settings_.on_message_complete = Net::onMessageComplete;
    parser_.data = this;
}
    
HTTPParser::~HTTPParser()
{
}
    
void HTTPParser::parse(const char* data, size_t size)
{
    http_parser_execute(&parser_, &settings_, data, size);
}
    
void HTTPParser::onURL(const std::string& url)
{
    URL_ = URL(url);
}

void HTTPParser::onStatus(int statusCode, const std::string& message)
{
    statusCode_ = statusCode;
    statusMessage_ = message;
}

void HTTPParser::onMessageBegin()
{
    messageBeginCallback_();
}

void HTTPParser::onMessageComplete()
{
    messageCompleteCallback_();
}

void HTTPParser::onHeaderField(const std::string& field)
{
    if(lastWasValue_) {
        lineNum_++;
        headerLines_[lineNum_-1].value = "";
        headerLines_[lineNum_-1].field = field;
    } else {
        headerLines_[lineNum_-1].field = headerLines_[lineNum_-1].field + field;
    }
    
    lastWasValue_ = false;
}

void HTTPParser::onHeaderValue(const std::string& value)
{
    if(!lastWasValue_) {
        headerLines_[lineNum_-1].value = value;
    } else {
        headerLines_[lineNum_-1].value = headerLines_[lineNum_-1].value + value;
    }
    
    lastWasValue_ = true;
}

void HTTPParser::onHeadersComplete()
{
    URLResponse response(URL_, statusCode_);
    for (std::vector<HeaderLine>::const_iterator iter = headerLines_.begin(); iter != headerLines_.end(); ++iter) {
        response.addHeader(iter->field, iter->value);
    }
    response.setContentLength(parser_.content_length);
    responseCompleteCalback_(std::move(response));
}

void HTTPParser::onBody(Packet&& buffer)
{
    dataCallback_(std::move(buffer));
}
}
}