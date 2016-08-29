#include "base/thread/Thread.h"
#include "net/TCPClient.h"
#include <iostream>
using namespace std;

int main()
{
	WukongBase::Base::Thread thread("httpget");
	thread.start();
	
    shared_ptr<WukongBase::Net::TCPSession> tcpSession;
	WukongBase::Net::TCPClient client(thread.messageLoop(), "onesmash.github.io", 80);
	client.setConnectCallback([](const shared_ptr<WukongBase::Net::TCPSession>& session) {
        session->setWriteCompleteCallback([&tcpSession](const WukongBase::Net::Packet& packet, bool success) {
        });
        session->setMessageCallback([](const shared_ptr<WukongBase::Base::IOBuffer>& buffer) {
            cout.write(buffer->data(), buffer->size());
        });
        session->setCloseCallback([](bool) {

        });
        char httpRequest[] = "GET / HTTP/1.1\r\nHost: onesmash.github.io\r\nUser-Agent: Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/3.0 Mobile/1A543 Safari/419.3\r\n\r\n";
		WukongBase::Net::Packet packet;
        packet.append(httpRequest, sizeof(httpRequest));
        session->send(std::move(packet));
        session->startRead();
        tcpSession = session;
    });
    client.connect();
    
    thread.join();
    
    return 0;
}