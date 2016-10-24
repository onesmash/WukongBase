//
//  TCPSession.cpp
//  AppCore
//
//  Created by Xuhui on 15/5/31.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#include "net/TCPSession.h"
#include "net/Packet.h"
#include "base/message_loop/MessageLoop.h"
#include <cassert>

namespace WukongBase {
namespace Net {
    
TCPSession::TCPSession(const std::shared_ptr<TCPSocket>& socket, const IPAddress& localAddress, const IPAddress& peerAddress)
    :   socket_(socket),
        localAddress_(localAddress),
        peerAddress_(peerAddress),
        state_(kConnected)
{
    socket_->setWriteCompleteCallback([this](const Packet& packet, bool success) {
        writeCompleteCallback_(packet, success);
    });
    socket_->setReadCompleteCallback([this](std::shared_ptr<Packet>& buffer) {
        readCompleteCallback_(buffer);
    });
    socket_->setCloseCallback([this](bool) {
        lock_.lock();
        if(state_ != kDisconnecting) {
            state_ = kDisconnecting;
            lock_.unlock();
            socket_->messageLoop()->postTask([this]() {
                {
                    std::lock_guard<std::mutex> guard(lock_);
                    state_ = kDisconnected;
                }
                closeCallback_(true);
                defaultCloseCallback_(true);
            });
        } else {
            state_ = kDisconnected;
            lock_.unlock();
            closeCallback_(true);
            defaultCloseCallback_(true);
        }
    });
}
    
TCPSession::~TCPSession()
{
    //assert(state_ == kDisconnected);
    if(state_ != kDisconnected) {
        std::shared_ptr<TCPSocket> socket = socket_;
        socket_->messageLoop()->postTask([socket]() {
            socket->kill();
        });
    }
    socket_ = nullptr;
}
    
void TCPSession::send(const Packet& packet)
{
    std::lock_guard<std::mutex> guard(lock_);
    if(state_ == kConnected) {
        if(Base::MessageLoop::current() == socket_->messageLoop()) {
            socket_->write(packet);
        } else {
            socket_->messageLoop()->postTask([this, packet]() {
                std::lock_guard<std::mutex> guard(lock_);
                if(state_ == kConnected) {
                    socket_->write(std::move(packet));
                }
            });
        }
    }
}

void TCPSession::send(Packet&& packet)
{
    std::lock_guard<std::mutex> guard(lock_);
    if(state_ == kConnected) {
        if(Base::MessageLoop::current() == socket_->messageLoop()) {
            socket_->write(std::move(packet));
        } else {
            std::shared_ptr<Packet> p(new Packet(std::move(packet)));
            socket_->messageLoop()->postTask([this, p]() {
                std::lock_guard<std::mutex> guard(lock_);
                if(state_ == kConnected) {
                    socket_->write(std::move(*p));
                }
            });
        }
    }
}
    
void TCPSession::startRead()
{
    if(Base::MessageLoop::current() == socket_->messageLoop()) {
        socket_->startRead();
    } else {
        socket_->messageLoop()->postTask([=]() {
            socket_->startRead();
        });
    }
    
}

void TCPSession::stopRead()
{
    if(Base::MessageLoop::current() == socket_->messageLoop()) {
        socket_->stopRead();
    } else {
        socket_->messageLoop()->postTask([=]() {
            socket_->stopRead();
        });
    }
}
    
void TCPSession::shutdown()
{
    if(Base::MessageLoop::current() == socket_->messageLoop()) {
        socket_->shutdown();
    } else {
        socket_->messageLoop()->postTask([=]() {
            socket_->shutdown();
        });
    }
}
    
void TCPSession::close()
{
    std::lock_guard<std::mutex> guard(lock_);
    if(state_ == kDisconnecting || state_ == kDisconnected) return;
    state_ = kDisconnecting;
    if(Base::MessageLoop::current() == socket_->messageLoop()) {
        socket_->close();
    } else {
        socket_->messageLoop()->postTask([=]() {
            socket_->close();
        });
    }
}
    
bool TCPSession::isClosed()
{
    std::lock_guard<std::mutex> guard(lock_);
    return state_ == kDisconnected;
}

}
}
