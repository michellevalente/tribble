#include <iostream>
#include <string>

using namespace std;

int main()
{
	string str = "";

	str += 	"HTTP/1.1 200 OK\r\n"
			"Date: Mon, 23 May 2005 22:38:34 GMT\r\n"
			"Server: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\r\n"
			"Last-Modified: Wed, 08 Jan 2003 23:11:55 GMT\r\n"
			"ETag: \"3f80f-1b6-3e1cb03b\"\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Content-Length: 138\r\n"
			"Accept-Ranges: bytes\r\n"
			"Connection: close\r\n"
			"\r\n"
			"<html>\r\n"
			"<head>\r\n"
			"  <title>An Example Page</title>\r\n"
			"</head>\r\n"
			"<body>\r\n"
			" Hello World, this is a very simple HTML document.\r\n"
			"</body>\r\n"
			"</html>\r\n";

	cout<<str;

	return 0;
}

