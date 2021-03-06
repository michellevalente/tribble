#include <iostream>
#include "Multicast.hpp"

using namespace std;

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

	if(multicast_addr.isMulticast()) {
		socket.joinGroup(multicast_addr);
		message = socket.receiveFrom(server);
		cout << "Message received: " << message << endl;
	} else {
		cout << "IP address is not a multicast address." << endl;
	}

	return 0;
}