#include <iostream>
#include <thread>

#include "Socket.hpp"
#include "StreamSocket.hpp"

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

class ConnectionHandler {
public:
    virtual void handle(StreamSocket& sock) = 0;
};

class ConnectionHandlerFactory {
public:
    virtual ConnectionHandler* getNewHandler() = 0;
};


class TCPServer {
private:    
    ConnectionHandlerFactory* cHandlerFactory;
    int port;
    int nThreads;
public:
    /** 
     *  Default value for nThreads is 0. If left as such, new threads will be created as needed
     *  to execute incoming requests.
    */
    TCPServer(ConnectionHandlerFactory* cHandlerFactory, uint16_t port, int nThreads = 0) : 
                        cHandlerFactory(cHandlerFactory), 
                        port(port),
                        nThreads(nThreads) {}

    void start() {
        std::cout << "Starting..." << "\n";

        StreamSocket wsock(port);
        while (true) {
            ConnectionHandler* cHandler = cHandlerFactory->getNewHandler();
            StreamSocket sock = wsock.accept();
            std::cout << "Incoming connection!\n";

            std::thread t([&](StreamSocket sock) {cHandler->handle(sock);}, sock);
            t.detach();
        }
    }
};

#endif