#include <iostream>
#include "../src/tribble.h"

//using namespace std;
//using namespace tribble;

int main() {
    StreamSocket wsock(5000);

    while (true) {
        StreamSocket sock = wsock.accept();

        //cout<<"  Connection established with "<<sock.getPeerAddress()<<endl; 

        string rcvd;

        while(sock >> rcvd) 
            sock << "Starting to echo\n" << rcvd << "Finishing echo\n";
    }

    return 0;
}