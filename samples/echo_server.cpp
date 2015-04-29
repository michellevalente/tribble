#include <iostream>
#include "tribble.h"


int main() {
    const short port = 5000;

    StreamSocket wsock(5000);

    std::cout << "Listening on port " << port << "\n\n";

    while (true) {
        StreamSocket sock = wsock.accept();
        string rcvd;

        do {
            error_code error;
            rcvd = sock.receive(1024);
            sock.send_all(rcvd);
        }
        while (rcvd.size() > 0);
        
    }

    return 0;
}
