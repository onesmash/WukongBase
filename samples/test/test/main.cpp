//
//  main.cpp
//  test
//
//  Created by Xuhui on 16/7/7.
//  Copyright © 2016年 Xuhui. All rights reserved.
//
#include "Thread.h"
#include "TCPSession.h"
#include "TCPClient.h"
#include <iostream>
#include <unistd.h>
using namespace std;

void connectCallback(const std::shared_ptr<WukongBase::Net::TCPSession>& session)
{
    WukongBase::Net::Packet packet;
    char httpReq[] = "GET /16/0706/12/BR9QB7DP00014PRF.html HTTP/1.1 \r\nHOST: news.163.com\r\n\r\n";
    packet.append(httpReq, sizeof(httpReq));
    session->send(std::move(packet));
    session->startRead();
}

void writeCallback(bool success)
{
    
}

void messageCallback(const std::shared_ptr<WukongBase::Net::TCPSession>& session, std::shared_ptr<WukongBase::Base::IOBuffer>& buffer)
{
    string text(buffer->data(), buffer->size()); 
    cout << text;
}

int main(int argc, const char * argv[]) {
    
    WukongBase::Base::Thread thread("test");
    thread.start();
    
    WukongBase::Net::TCPClient client(thread.messageLoop(), WukongBase::Net::IPAddress("122.228.84.23", 80));
    client.setConnectCallback(connectCallback);
    client.setWriteCompleteCallback(writeCallback);
    client.setMessageCallback(messageCallback);
    client.connect();
    
    sleep(INT_MAX);
    
    return 0;
}
