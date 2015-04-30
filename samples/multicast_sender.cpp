#include <iostream>
#include "Multicast.hpp"

using namespace std;

int main()
{
	string ip_address;
	int port;
	string message;

	cout << "Multicast address: ";
	cin >> ip_address;

	cout << "Port number: ";
	cin >> port;

	cout << "Message to send: ";
	cin >> message;

	Multicast socket;

	socket.setTTL(1);

	for(;;)
	{
		HostAddr serverAddr(ip_address, port);
		socket.sendTo(message, serverAddr);
	}

	return 0;
}