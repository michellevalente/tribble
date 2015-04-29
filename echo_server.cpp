#include <iostream>
#include "tribble.h"

//using namespace std;
//using namespace tribble;

int main() {
    StreamSocket wsock(5000);

    while (true) {
        StreamSocket sock = wsock.accept();
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