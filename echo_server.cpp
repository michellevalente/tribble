#include <iostream>
#include "socket.hpp"

<<<<<<< HEAD
using namespace std;

int main() {
    StreamSocket wsock(8080);

    while (true) {
        StreamSocket sock = wsock.accept();
        string rcvd;

        do
        {
=======
//using namespace std;
//using namespace tribble;

int main() {
    StreamSocket wsock(5000);

    while (true) {
        StreamSocket sock = wsock.accept();
        while(true) {
>>>>>>> 4084cc325cb4a631b0c70da8979a51b0719e97da
            error_code error;
            rcvd = sock.receive(1024, error);
            sock.send_all(rcvd);

        }
        while(rcvd.size()>0);

    }
    
    return 0;
}
