#include <iostream>
#include "DatagramSocket.hpp"
#include "HostAddr.hpp"

using namespace std;

int main()
{
   DatagramSocket sock(5000);
   HostAddr client;
   string message;
   
   for(;;) {
      message = sock.receiveFrom( client);
      cout << "msg: " << message << endl;
      sock.sendTo(message, client);
   }

   return 0;
}
