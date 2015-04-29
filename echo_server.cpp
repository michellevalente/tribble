#include <iostream>
#include "socket.hpp"

using namespace std;

int main() {
    StreamSocket wsock(8080);

    while (true) {
        StreamSocket sock = wsock.accept();
        string rcvd;

        do
        {
            error_code error;
            rcvd = sock.receive(1024, error);
            sock.send_all(rcvd);

        }
        while(rcvd.size()>0);

    }

    return 0;
}
