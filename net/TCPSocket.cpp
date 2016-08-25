//
//  TCPSocket.cpp
//  AppCore
//
//  Created by Xuhui on 15/5/26.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#include "net/TCPSocket.h"
#include "net/Packet.h"
#include "base/message_loop/MessageLoop.h"
#include <cstdlib>
#include <cassert>

namespace WukongBase {
namespace Net {
    
void onAllocBuf(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    TCPSocket* socket = (TCPSocket*)handle->data;
    if(socket == nullptr) return;
    buf->len = socket->readBufSize();
    buf->base = socket->readBuf();
}
    
void onConnectReq(uv_stream_t* server, int status)
{
    TCPSocket* serverSocket = (TCPSocket*)server->data;
    if(serverSocket == nullptr) return;
    if(status == 0) {
        serverSocket->didReciveConnectRequest();
    } else {
        
    }
}
    
void onConnectComplete(uv_connect_t* req, int status)
{
    TCPSocket* socket = (TCPSocket*)req->handle->data;
    if(socket == nullptr) return;
    bool sucess = status >= 0;
    socket->didConnectComplete(sucess);
}
    
void onReadComplete(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    TCPSocket* socket = (TCPSocket*)stream->data;
    if(socket == nullptr) return;
    if(nread > 0) {
        std::shared_ptr<Packet> packet(new Packet());
        packet->append(buf->base, nread);
        socket->didReadComplete(packet);
    } else if(nread < 0) {
        socket = (TCPSocket*)stream->data;
        socket->close();
    }
}
    
void onWriteComplete(uv_write_t* req, int status)
{
    TCPSocket* socket = (TCPSocket*)req->handle->data;
    if(socket == nullptr) return;
    bool sucess = status >= 0;
    socket->didWriteComplete(req, sucess);
    if(status < 0 && status != UV_ECANCELED && status != UV_SHUTDOWN) {
        socket->close();
    }
}
    
void onShutdownComplete(uv_shutdown_t* req, int status)
{
    
}
    
void onCloseComplete(uv_handle_t* handle)
{
    TCPSocket* socket = (TCPSocket*)handle->data;
    handle->data = nullptr;
    socket->didCloseComplete();
}
    
TCPSocket::TCPSocket(): closed_(true)
{
}
    
TCPSocket::~TCPSocket()
{
    assert(closed_);
    tcpSocket_.data = nullptr;
}
    
int TCPSocket::open(Base::MessageLoop* messageLoop)
{
    messageLoop_ = messageLoop;
    int res = -uv_tcp_init(&messageLoop_->eventLoop(), &tcpSocket_);
    tcpSocket_.data = this;
    closed_ = false;
    return res;
}
    
int TCPSocket::bind(const IPAddress& address)
{
    return -uv_tcp_bind(&tcpSocket_, (const struct sockaddr*)address.sockAddress(), 0);
}
    
int TCPSocket::listen(int backlog)
{
    return -uv_listen((uv_stream_t*)&tcpSocket_, backlog, onConnectReq);
}
    
int TCPSocket::accept(TCPSocket& socket)
{
    return -uv_accept((uv_stream_t*)&tcpSocket_, (uv_stream_t*)socket.tcpSocketHandle());
}
    
int TCPSocket::connect(const IPAddress& address)
{
    return -uv_tcp_connect(&tcpConnectReq_, &tcpSocket_, address.sockAddress(), onConnectComplete);
}
    
int TCPSocket::startRead()
{
    return -uv_read_start((uv_stream_t*)&tcpSocket_, onAllocBuf, onReadComplete);
}
    
int TCPSocket::stopRead()
{
    return -uv_read_stop((uv_stream_t*)&tcpSocket_);
}
    
int TCPSocket::write(const Packet& packet)
{
    WriteRequest* writeRequest = new WriteRequest;
    writeRequest->packet = packet;
    writeRequest->writeRequest.data = writeRequest;
    uv_buf_t buf = uv_buf_init((char*)writeRequest->packet.data(), (unsigned)writeRequest->packet.size());
    std::shared_ptr<WriteRequest> ptr = std::shared_ptr<WriteRequest>(writeRequest);
    writeRequestMap_[writeRequest] = ptr;
    return -uv_write(&writeRequest->writeRequest, (uv_stream_t*)&tcpSocket_, &buf, 1, onWriteComplete);
}
    
int TCPSocket::write(Packet&& packet)
{
    WriteRequest* writeRequest = new WriteRequest;
    writeRequest->packet = std::move(packet);
    writeRequest->writeRequest.data = writeRequest;
    uv_buf_t buf = uv_buf_init((char*)writeRequest->packet.data(), (unsigned)writeRequest->packet.size());
    std::shared_ptr<WriteRequest> ptr = std::shared_ptr<WriteRequest>(writeRequest);
    writeRequestMap_.insert({{writeRequest, ptr}});
    return -uv_write(&writeRequest->writeRequest, (uv_stream_t*)&tcpSocket_, &buf, 1, onWriteComplete);
}
    
int TCPSocket::shutdown()
{
    return -uv_shutdown(&tcpShutdownReq_, (uv_stream_t*)&tcpSocket_, onShutdownComplete);
}
    
int TCPSocket::close()
{
    // 1 closed, 0 closing
    if(!uv_is_closing((uv_handle_t*)&tcpSocket_)) {
        uv_close((uv_handle_t*)&tcpSocket_, onCloseComplete);
        return 0;
    } else {
        return closed_ ? 1 : 0;
    }
}
    
IPAddress TCPSocket::getLocalAddress()
{
    sockaddr_storage addr;
    int len = 0;
    uv_tcp_getsockname(&tcpSocket_, (sockaddr*)(&addr), &len);
    return IPAddress((sockaddr*)(&addr));
}

IPAddress TCPSocket::getPeerAddress()
{
    sockaddr_storage addr;
    int len = 0;
    uv_tcp_getpeername(&tcpSocket_, (sockaddr*)(&addr), &len);
    return IPAddress((sockaddr*)(&addr));
}

void TCPSocket::didReciveConnectRequest()
{
    acceptCallback_();
}
    
void TCPSocket::didConnectComplete(bool success)
{
    if(!success) {
        closed_ = true;
    }
    connectCallback_(success);
}
    
void TCPSocket::didReadComplete(std::shared_ptr<Packet>& packet)
{
    readCompleteCallback_(packet);
}
    
void TCPSocket::didWriteComplete(TCPSocketWriteRequest* request, bool success)
{
    WriteRequest* writeRequest = (WriteRequest*)request->data;
    writeCompleteCallback_(writeRequest->packet, success);
    writeRequestMap_.erase(writeRequest);
}
    
void TCPSocket::didCloseComplete()
{
    closed_ = true;
    tcpSocket_.data = nullptr;
    closeCallback_(true);
    
}
    
    
}
}