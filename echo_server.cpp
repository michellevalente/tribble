#include <iostream>
#include "tribble.h"

using namespace std;
using namespace tribble;

int main() {
    Socket wsock(8080);

    while (true) {
        Socket sock = wsock.accept();

        while(true) {
            error_code error;
            string rcvd = sock.receive(1024, error);
            // if (error = error::eof)
                // break;
            sock.send_all(rcvd);
        }
    }

    return 0;
}