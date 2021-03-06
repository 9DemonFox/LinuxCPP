#include "socket_handler.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <new>
#include "reactor.h"
#include<iostream>
SocketHandler::SocketHandler(Handle fd) :
        _socket_fd(fd) {
    _buf = new (std::nothrow)char[MAX_SIZE];
    assert(_buf != NULL);
    memset(_buf, 0, MAX_SIZE);
}

SocketHandler::~SocketHandler() {
    close(_socket_fd);
    delete[] _buf;
}

void SocketHandler::handle_read() {
    if (read(_socket_fd, _buf, MAX_SIZE) > 0) {
        std::cout<<_buf<<std::endl;
        write(_socket_fd, _buf, strlen(_buf));
    } 
    EventHandler* h = new (std::nothrow)SocketHandler(_socket_fd);
    assert(h != NULL);
    Reactor& r = Reactor::get_instance();
    r.regist(h, ReadEvent);
    // handle_error();
}

void SocketHandler::handle_write() {
    // do nothing
}

void SocketHandler::handle_error() {
    Reactor& r = Reactor::get_instance();
    r.remove(this);
}
