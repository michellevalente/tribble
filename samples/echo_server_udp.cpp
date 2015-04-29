#include <iostream>
#include "tribble.h"

int main()
{
   DatagramSocket sock(5000);
   HostAddr client;
   string message;
   
   for(;;) {
      message = sock.receiveFrom(1024, client);
      cout << "msg: " << message << endl;
      sock.sendTo(message, client);
   }

   return 0;
}