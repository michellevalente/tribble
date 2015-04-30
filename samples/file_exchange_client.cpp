#include <iostream>
#include <fstream>
#include "StreamSocket.hpp"
#define PORT 5000

using namespace std;

int main()
{
	string filename, ip;
	char buffer[1000];
	int port;
	int size;

	FILE * inputFile;
	StreamSocket socket;

	cout << "File name: ";
	cin >> filename;

	cout << "Server address: ";
	cin >> ip;

	cout << "Port number: ";
	cin >> port;
	HostAddr server(ip, port);

	socket.connect(server);

	std::ifstream inputfile;

	inputfile.open(filename);


	int len = 0;

	while(!inputfile.eof())
	{
		inputfile.read(buffer, 1000);
		socket.send_all(string(buffer));
	}	

	inputfile.close();

	
}