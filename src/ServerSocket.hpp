#include <system_error>
#include "Socket.hpp"
#include "BufferedSocket.hpp"

#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

class ServerSocket: public Socket
{

public:
    ServerSocket();
    ServerSocket(uint16_t port_number);
    ServerSocket accept();
    BufferedSocket acceptBuffered();
    void send_all(const string& to_send) const;
    string getPeerAddress() const;
    unsigned short getPeerPort() const;
    int operator >>( std::string& s ) const;
    const ServerSocket& operator << ( const std::string& s ) const;
};


void ServerSocket::send_all(const string& to_send) const
{
    int length = to_send.length();
    const char *ptr = &to_send[0];
    while (length > 0) {
        int i = ::send(c_socket, ptr, length, 0);
        if (i < 1) 
            throw NetworkException("Error sending message.", errno);
        ptr += i;
        length -= i;
    }
}

ServerSocket::ServerSocket()
{
    if ((c_socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw NetworkException("Socket creation failed.", errno);
}

ServerSocket::ServerSocket(uint16_t port)
{
    this->port_number = port;

    if ((c_socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw NetworkException("Socket creation failed.", errno);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));       /* Zero out structure */
    addr.sin_family = AF_INET;                /* Internet address family */
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    addr.sin_port = htons(this->port_number);              /* Local port */

    /* Bind to the local address */
    if (::bind(c_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        throw NetworkException("Socket bind failed.", errno);

    /* Mark the socket so it will listen for incoming connections */
    if (::listen(c_socket, MAXPENDING) < 0)
        throw NetworkException("Socket listen failed.", errno);
}

ServerSocket ServerSocket::accept()
{
    ServerSocket sock;
    struct sockaddr_in addr;
    unsigned int addrLen = sizeof(addr); 
    sock.c_socket = ::accept(c_socket, (struct sockaddr *)&addr, &addrLen);

    return sock;
}

BufferedSocket ServerSocket::acceptBuffered()
{
    BufferedSocket sock;
    struct sockaddr_in addr;
    unsigned int addrLen = sizeof(addr); 
    sock.setC_Socket( ::accept(c_socket, (struct sockaddr *)&addr, &addrLen) );

    return sock;
}

string ServerSocket::getPeerAddress() const
{
    sockaddr addr;
    unsigned int addrLen = sizeof(addr);

    if(getpeername(c_socket, &addr, &addrLen) < 0)
    {
        throw NetworkException("Getting address failed.", errno);
    }

    return string(inet_ntoa(((sockaddr_in*) &addr)->sin_addr));
}

uint16_t ServerSocket::getPeerPort() const
{
    sockaddr addr;
    unsigned int addrLen = sizeof(addr);

    if(getpeername(c_socket, &addr, &addrLen) < 0)
    {
        throw NetworkException("Getting address failed.", errno);
    }

    return ((sockaddr_in*) &addr)->sin_port;
}

const ServerSocket& ServerSocket::operator<<(const std::string& s) const
{
    send_all(s);
    return *this;
}

#define CHUNK_SIZE 10

int ServerSocket::operator>>(std::string& s) const
{
    char *chunking = new char[CHUNK_SIZE];
    int chunked;

    s = "";

    do {
        chunked = recv( c_socket, chunking, CHUNK_SIZE, 0 );

        if(chunked<0) {
            int _errno = errno;
            delete [] chunking;
            throw NetworkException("Receiving from socket failed", _errno);
        }

        if(chunked>0) {
            s.insert( s.size() , chunking , chunked );
        }
    }
    while(chunked > 0); // zero indicates end of connection (ctrl-D)

    delete [] chunking;
    return s.size();
}

#endif
