#include <iostream>
#include "../src/ServerSocket.hpp"
#include "../src/BufferedSocket.hpp"

using namespace std;
//using namespace tribble;

int main() {
    ServerSocket wsock(5000);

    while (true) {
        BufferedSocket sock = wsock.acceptBuffered();

        //cout<<"  Connection established with "<<sock.getPeerAddressInt()<<":"<<sock.getPeerPort()<<endl; 
		cout<<"  Connection established with "<<sock.getPeerAddress()<<endl; 

        string rcvd;

        /*

            One possible usage: (word by word)

            while(sock>>rcvd)
                cout<<"["<<rcvd<<"]"<<endl;

        */

        /* 

            Another possible usage: 

            while(sock.getNextLine(rcvd))
            {
                cout<<rcvd<<endl;
            }
        */

        while(sock.getNextLine(rcvd))
        {
            sock<<"echoing: "<<rcvd<<"\r\n";
            sock.flush();
        }


    }

    return 0;
}