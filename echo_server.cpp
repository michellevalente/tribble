#include <iostream>
#include "tribble.h"

//using namespace std;
//using namespace tribble;

int main() {
    StreamSocket wsock(5000);

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