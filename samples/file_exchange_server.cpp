#include <iostream>
#include <fstream>
#include "StreamSocket.hpp"
#include "Buffer.hpp"
#define PORT 5000

using namespace std;

int main()
{
	string filename;
	int size;

	FILE * inputFile;
	StreamSocket socket(PORT);

	cout << "File name: ";
	cin >> filename;

	std::ofstream file(filename, std::ios::binary);

	StreamSocket clientSocket = socket.accept();

	Buffer buffer;

	for(;;)
	{
		clientSocket.receive(buffer);

		if(buffer.getSize() != 0)
			file.write((char*) buffer.getArray(), buffer.getSize());
		else {
			file.close();
			break;
		}

	}
}