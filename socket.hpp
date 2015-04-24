#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <netdb.h>      /* for gethostbyname() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <unistd.h>     /* for close() */
#include <signal.h>     /* for signal() */
#include <sys/stat.h>   /* for stat() */
#include <string.h>     /* for memset() */
#include <vector>
#include <string>
#include <system_error>

using namespace std;

class Socket 
{
protected:
    int MAXPENDING = 10;
    int c_socket;
    int port_number;
    // Socket();

public:
    string receive(int buffer_size, error_code error);
    void send_all(string to_send);
    
};

class StreamSocket: public Socket
{

public:
    StreamSocket();
    StreamSocket(int port_number);
    StreamSocket accept();
};

class DatagramSocket: public Socket
{

public:
    DatagramSocket();
    DatagramSocket(int port_number);
};