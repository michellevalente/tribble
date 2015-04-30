#include <iostream>
#include "DatagramSocket.hpp"

using namespace std;

int main(int argc, char** argv)
{
	DatagramSocket sock;
	string ip = string(argv[1]);
	string port_str = string(argv[2]);
	int port = stoi(port_str);
	HostAddr server(ip, port);
	char message[1024];
	for(;;)
	{
		cin >> message;
		sock.sendTo(string(message), server);
		cout << "msg: " << sock.receiveFrom(server, 1024) << endl;
	}

}
