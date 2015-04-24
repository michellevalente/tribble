#include <iostream>
#include <thread>
#include "tribble.h"

using namespace std;
using namespace tribble;


int gi;


class ConnectionHandler {
public:
    virtual void handle(Socket& sock) = 0;
};

class ConnectionHandlerFactory {
public:
    virtual ConnectionHandler* getNewHandler() = 0;
};


class TCPServer {
private:    
    ConnectionHandlerFactory* cHandlerFactory;
    int port;
public:
    TCPServer(ConnectionHandlerFactory* cHandlerFactory, int port) : 
                        cHandlerFactory(cHandlerFactory), 
                        port(port) {}

    void start() {
        cout << "Starting..." << endl;

        Socket wsock(port);
        while (true) {  
            ConnectionHandler* cHandler = cHandlerFactory->getNewHandler();
            Socket sock = wsock.acpt();

            std::thread t([&](Socket sock) {cHandler->handle(sock);}, sock);
            t.detach();
            // cHandler->handle(sock);
        }
    }
};























class MyConnectionHandler : public ConnectionHandler {
public:
    MyConnectionHandler() = default;

    void handle(Socket& sock) override {
        while(true) {
            error_code error;
            string rcvd = sock.handlereceive(1024, error);
            sock.send_all(rcvd);
        }
    }
};

class MyConnectionHandlerFactory : public ConnectionHandlerFactory {
public:
    ConnectionHandler* getNewHandler() override {
        return new MyConnectionHandler();
    }
};


int main() {
    TCPServer tcpServer(new MyConnectionHandlerFactory(), 8082);
    tcpServer.start();
}