//
//  main.cpp
//  httpget
//
//  Created by Xuhui on 16/7/11.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "base/thread/Thread.h"
#include "net/TCPClient.h"
#include <iostream>
using namespace std;

int main()
{
    WukongBase::Base::Thread thread("httpget");
    thread.start();
    
    WukongBase::Net::TCPClient client(thread.messageLoop(), "onesmash.github.io", 80);
    client.setConnectCallback([](const std::shared_ptr<WukongBase::Net::TCPSession>& session){
        char httpRequest[] = "GET /index.html HTTP/1.1\r\nHost: onesmash.github.io\r\nUser-Agent: Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/3.0 Mobile/1A543 Safari/419.3\r\n\r\n";
        WukongBase::Net::Packet packet;
        packet.append(httpRequest, sizeof(httpRequest));
        session->send(std::move(packet));
        session->startRead();
    });
    client.setWriteCompleteCallback([](const std::shared_ptr<WukongBase::Net::TCPSession>& session, bool success) {
        
    });
    client.setMessageCallback([](const std::shared_ptr<WukongBase::Net::TCPSession>& session, std::shared_ptr<WukongBase::Base::IOBuffer>& buffer) {
        cout << buffer->data() << endl;
    });
    client.connect();
    
    thread.join();
    
    return 0;
}
