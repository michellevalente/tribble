#include <iostream>
#include <fstream>
#include "StreamSocket.hpp"
#define PORT 5000

using namespace std;

int main()
{
	string buffer;
	string filename;
	int size;

	FILE * inputFile;
	StreamSocket socket(PORT);

	cout << "File name: ";
	cin >> filename;

	std::ofstream out(filename);

	StreamSocket clientSocket = socket.accept();

	for(;;)
	{
		string buffer;
		buffer = clientSocket.receive(1024);
		if(buffer != "")
			out << buffer;
		else {
			out.close();
			break;
		}

	}
}