#include <iostream>
#include "tribble.h"

//using namespace std;
//using namespace tribble;

int main() {
    StreamSocket wsock(5000);

    while (true) {
        StreamSocket sock = wsock.accept();
        string rcvd;
        
        while(sock >> rcvd) 
            sock << rcvd;
    }

    return 0;
}