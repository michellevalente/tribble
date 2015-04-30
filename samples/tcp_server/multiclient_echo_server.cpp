#include <iostream>
#include <thread>

#include "TCPServer.hpp"
#include "Socket.hpp"

using namespace std;

class MyConnectionHandler : public ConnectionHandler {
public:
    MyConnectionHandler() = default;

    void handle(StreamSocket& sock) override {
        while(true) {
            cout << "handle called.\n";
            string rcvd = sock.receive();
            cout << "received data\n";
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