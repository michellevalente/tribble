#include <iostream>
#include "ServerSocket.hpp"
#include "BufferedSocket.hpp"

using namespace std;
//using namespace tribble;

int main() {

	string aux;

    BufferedSocket sock;

	sock.connect("127.0.0.1",5000);


	while(getline(cin,aux))
	{
		sock<<aux<<endN;
		sock.flush();

		sock.getNextLine(aux);
		cout<<aux<<endl;

	}

    return 0;
}