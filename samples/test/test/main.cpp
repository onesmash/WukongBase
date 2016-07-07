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
    char httpReq[] = "GET / HTTP/1.1 \r\nHOST: onesmash.github.io\r\nUser-Agent: Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/3.0 Mobile/1A543 Safari/419.3\r\n\r\n";
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
    
    WukongBase::Net::TCPClient client(thread.messageLoop(), WukongBase::Net::IPAddress("151.101.16.133", 80));
    client.setConnectCallback(connectCallback);
    client.setWriteCompleteCallback(writeCallback);
    client.setMessageCallback(messageCallback);
    client.connect();
    
    sleep(INT_MAX);
    
    return 0;
}
