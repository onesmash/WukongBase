#include "base/thread/Thread.h"
#include "net/TCPClient.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	WukongBase::Base::Thread thread("client");
	thread.start();
	
	WukongBase::Net::TCPClient client(thread.messageLoop(), WukongBase::Net::IPAddress("127.0.0.1", 9000));
	client.setConnectCallback([](const std::shared_ptr<WukongBase::Net::TCPSession>& session){
        string message;
        while(getline(cin, message)) {
            WukongBase::Net::Packet packet;
            packet.append((void*)message.c_str(), message.size());
            session->send(std::move(packet));
        }
    });
    client.setWriteCompleteCallback([](const std::shared_ptr<WukongBase::Net::TCPSession>& session, bool success) {
        
    });
    client.setMessageCallback([](const std::shared_ptr<WukongBase::Net::TCPSession>& session, std::shared_ptr<WukongBase::Base::IOBuffer>& buffer) {
        
    });
    client.connect();
    
    thread.join();
    
    return 0;
}