#include <iostream>
#include <string>
#include "StreamSocket.hpp"

using namespace std;

int main()
{
	string serverAddr;
	int port;

	cout << "Server address: ";
	cin >> serverAddr;

	cout << "Port number: ";
	cin >> port;

	HostAddr server(serverAddr, port);
	StreamSocket socket;

	socket.connect(server);

	cout << socket.receive() << endl;

	return 0;
}