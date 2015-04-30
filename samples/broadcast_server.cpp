#include <iostream>

#include "Broadcast.hpp"
#include "HostAddr.hpp"

int main()
{
	int port;
	Broadcast sock;
	string message;
	HostAddr broadcast;

	cout << "Port number: ";
	cin >> port;

	broadcast.setPort(port);

	for(;;)
	{
      	cout << "Message: ";
      	cin >> message;
      	sock.sendTo(message, broadcast);
	}
}