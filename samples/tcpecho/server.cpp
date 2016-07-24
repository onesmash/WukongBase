#include "net/TCPServer.h"
#include "base/thread/Thread.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	WukongBase::Base::Thread thread("server");
	thread.start();
	
	WukongBase::Net::TCPServer server(thread.messageLoop(), WukongBase::Net::IPAddress("127.0.0.1", 9000));
	server.setConnectCallback([](const std::shared_ptr<WukongBase::Net::TCPSession>& session){
        session->startRead();
    });
    server.setWriteCompleteCallback([](const std::shared_ptr<WukongBase::Net::TCPSession>& session, bool success) {
        
    });
    server.setMessageCallback([](const std::shared_ptr<WukongBase::Net::TCPSession>& session, const std::shared_ptr<WukongBase::Base::IOBuffer>& buffer) {
        cout.write(buffer->data(), buffer->size());
        cout << endl;
    });
    server.start();
    thread.join();
    
    return 0;
}