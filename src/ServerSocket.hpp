#include <system_error>
#include "Socket.hpp"
#include "StreamSocket.hpp"
#include "BufferedSocket.hpp"

#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

/** \brief Represents a Stream Server Socket 
 */
class ServerSocket: public Socket
{

public:
    ServerSocket(uint16_t port_number);
    StreamSocket accept();
    BufferedSocket acceptBuffered();
    string getPeerAddress() const;
    unsigned short getPeerPort() const;
};

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

/** \brief Accepts a new connection and returns a new StreamSocket
 */
StreamSocket ServerSocket::accept()
{
    StreamSocket socket;
    struct sockaddr_in addr;
    unsigned int addrLen = sizeof(addr); 
    socket.c_socket = ::accept(c_socket, (struct sockaddr *)&addr, &addrLen) ;

    return socket;
}

BufferedSocket ServerSocket::acceptBuffered()
{
    BufferedSocket socket;
    struct sockaddr_in addr;
    unsigned int addrLen = sizeof(addr); 
    socket.c_socket = ::accept(c_socket, (struct sockaddr *)&addr, &addrLen) ;

    return socket;
}

/** \brief Get the peer IP address.
 */
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

/** \brief Get the Peer port number.
 */
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


#endif
