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
        cout << "connected" << endl;
        char httpRequest[] = "GET / HTTP/1.1\r\nHost: onemsmash.github.io\r\nUser-Agent: Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/3.0 Mobile/1A543 Safari/419.3\r\n\r\n";
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