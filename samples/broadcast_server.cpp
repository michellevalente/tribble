#include <iostream>

#include "Broadcast.hpp"
#include "HostAddr.hpp"

using namespace std;

int main()
{
	int port;
	Broadcast sock;
	string message;
	IPAddr localhost("127.0.0.1");
	IPAddr broadcastAddr = localhost.generateBroadcast("0.255.255.255");

	std::cout << "Port number: ";
	std::cin >> port;

	HostAddr broadcast(broadcastAddr.stringIP(), port);



	for(;;)
	{
      	std::cout << "Message: ";
      	std::cin >> message;
      	sock.sendTo(message, broadcast);
	}
}