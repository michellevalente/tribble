#include <iostream>
#include "Multicast.hpp"

int main()
{
	string ip_address;
	int port;
	HostAddr server;
	string message;

	cout << "Multicast address: ";
	cin >> ip_address;

	cout << "Port number: ";
	cin >> port;

	Multicast socket(port);

	IPAddr multicast_addr(ip_address);

	socket.joinGroup(multicast_addr);

	message = socket.receiveFrom(1024, server);

	cout << "Message received: " << message << endl;
	
	return 0;

}