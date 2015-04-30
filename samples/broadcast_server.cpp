#include <iostream>

#include "Broadcast.hpp"
#include "HostAddr.hpp"

int main()
{
	int port;
	Broadcast sock;
	string message;
	HostAddr broadcast;

	std::cout << "Port number: ";
	std::cin >> port;

	broadcast.setPort(port);

	for(;;)
	{
      	std::cout << "Message: ";
      	std::cin >> message;
      	sock.sendTo(message, broadcast);
	}
}