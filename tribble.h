
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

namespace tribble {
    class Socket {
    private:
        int MAXPENDING = 10;
        int c_socket;
        int port_number;
    public:
        Socket();
        Socket(int port_number);
        string receive(int buffer_size, error_code error);
        void send_all(string to_send);
        Socket acpt();
    };
}

using namespace tribble;

Socket::Socket() {
    if ((c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw std::invalid_argument("TODO: change this exception type and description");
}

Socket::Socket(int port_number) {
    this->port_number = port_number;

    if ((c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw std::invalid_argument("TODO: change this exception type and description");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));       /* Zero out structure */
    addr.sin_family = AF_INET;                /* Internet address family */
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    addr.sin_port = htons(port_number);              /* Local port */

    /* Bind to the local address */
    if (bind(c_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::invalid_argument("TODO: change this exception type and description");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(c_socket, MAXPENDING) < 0)
        throw std::invalid_argument("TODO: change this exception type and description");
}

string Socket::receive(int buffer_size, error_code error) {
    char* buffer = new char[buffer_size+1];
    int sz = recv(c_socket, buffer, sizeof(buffer), 0);

    buffer[sz] = '\0';
    string str(buffer);
    delete[] buffer;

    return str;
}

void Socket::send_all(string to_send) {
    int length = to_send.length();
    char *ptr = &to_send[0];
    while (length > 0)
    {
        int i = send(c_socket, ptr, length, 0);
        if (i < 1) 
            throw std::invalid_argument("TODO: change this exception type and description");
        ptr += i;
        length -= i;
    }
}

Socket Socket::acpt() {
    Socket sock;
    struct sockaddr_in addr;
    unsigned int addrLen = sizeof(addr); 
    sock.c_socket = accept(c_socket, (struct sockaddr *)&addr, &addrLen);

    return sock;
}

/* Function to translate hostname to ip address. Default ip_version returns 
 * ipv4 and ipv6 addresses */
vector<string> dns_lookup(const string hostname, int version_ip = 0){

    vector<string> ip; 
    struct addrinfo hints;
    struct addrinfo *result, *it;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));

    if(version_ip == 6)
        hints.ai_family = AF_INET6;
    else if(version_ip == 4)
        hints.ai_family = AF_INET;
    else
        hints.ai_family = AF_UNSPEC;

    hints.ai_socktype = SOCK_STREAM;

    s = getaddrinfo(hostname.c_str(), NULL, &hints, &result);

    if (s != 0) {
         throw std::invalid_argument("TODO: change this exception type and description");
    }

    for(it = result;it != NULL; it = it->ai_next) {

        void * ip_address;
        struct sockaddr_in * ipv4;
        struct sockaddr_in6 *ipv6;
        char ip_string[INET6_ADDRSTRLEN];

        // IPV4
        if (it->ai_family == AF_INET) { 
            ipv4 = (struct sockaddr_in *) it->ai_addr;
            ip_address = &(ipv4->sin_addr);
        } 
        // IPV6
        else { 
            ipv6 = (struct sockaddr_in6 *) it->ai_addr;
            ip_address = &(ipv6->sin6_addr);
        }
 
        // convert the IP to a string
        inet_ntop(it->ai_family, ip_address, ip_string, sizeof (ip_string));

        // Put string in the vector
        ip.push_back(ip_string);
    }

    return ip;

}