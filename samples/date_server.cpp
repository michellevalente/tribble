#include <iostream>
#include <string>
#include "StreamSocket.hpp"
#include "ServerSocket.hpp"

using namespace std;

int main()
{
	int port;
	time_t t;
	struct tm * t_info;
	string date;
	char buffer[100];

	time(&t);
	t_info = localtime(&t);

	strftime(buffer,100,"%d-%m-%Y %I:%M:%S",t_info);

	date = string(buffer);

	cout << "Port number: ";
	cin >> port;

	ServerSocket socket(port);

	for(;;)
	{
		StreamSocket clientSocket = socket.accept();
		clientSocket.send_all(date);
	}
}