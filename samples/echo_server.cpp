#include <iostream>
#include "StreamSocket.hpp"
#include "ServerSocket.hpp"


int main() {
    const short port = 5000;

    ServerSocket wsock(5000);

    std::cout << "Listening on port " << port << "\n\n";

    //Serves a single client
    StreamSocket sock = wsock.accept();
    string rcvd;

    do {
        rcvd = sock.receive(1024);
        sock.send_all(rcvd);
    }
    while (rcvd.size() > 0);

    std::cout << "Connection closed" << "\n";

    return 0;
}
