#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <netdb.h>      /* for gethostbyname() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <unistd.h>     /* for close() */
#include <signal.h>     /* for signal() */
#include <sys/stat.h>   /* for stat() */
#include <string.h>     /* for memset() */
#include <fcntl.h>      /* for fcntl() */
#include <vector>
#include <string>
#include <system_error>
#include "Error.hpp"
#include "HostAddr.hpp"

using std::string;

#ifndef SOCKET_H
#define SOCKET_H


/** \brief Class to represent a socket.
 *
 *  Detailed here.
 */
class Socket 
{
protected:
    int MAXPENDING;
    int c_socket;
    int port_number;

public:
    Socket()
    {
        MAXPENDING = 10;
    }

    ~Socket()
    {
        if (close(c_socket)!=0) 
            throw NetworkException("Socket could not be closed.", errno);
    }

    string receive(int buffer_size);
    //int select(vector<Socket> readSocket, vector<Socket> writeSocket, struct timeval time );
    void setSendSize(int bytes);
    void setReceiveSize(int bytes);
    int getSendSize();
    int getReceiveSize();
    void setSendTimeOut(struct timeval time);
    void setReceiveTimeOut(struct timeval time);
    IPAddr localAddr();
    int localPort();
    void setBlocking(bool blocking);
    
};

/** 
 *  Sets the maximum socket send buffer.
 */
void Socket::setSendSize(int bytes)
{

    if ( setsockopt(c_socket, SOL_SOCKET, SO_SNDBUF, &bytes, sizeof(bytes)) != 0 )
        throw NetworkException("Error setting send buffer size.", errno);
}

/** 
 *  Sets the maximum socket receiver buffer.
 */
void Socket::setReceiveSize(int bytes)
{
    if ( setsockopt(c_socket, SOL_SOCKET, SO_RCVBUF, &bytes, sizeof(bytes)) != 0 )
        throw NetworkException("Error setting receive buffer size.", errno);
}

/** 
 *  Returns the maximum socket send buffer.
 */
int Socket::getSendSize()
{
    int size;
    socklen_t len=sizeof(size);
    if ( getsockopt(c_socket, SOL_SOCKET, SO_SNDBUF, &size, &len) != 0 )
        throw NetworkException("Error getting send buffer size.", errno);

    return size;
}

/** 
 *  Returns the maximum socket receiver buffer.
 */
int Socket::getReceiveSize()
{
    int size;
    socklen_t len=sizeof(size);
    if ( getsockopt(c_socket, SOL_SOCKET, SO_RCVBUF, &size, &len) != 0 )
        throw NetworkException("Error getting receive buffer size.", errno);

    return size;
}

/**
 *  Sets the socket send timeout.
 */
void Socket::setSendTimeOut(struct timeval t)
{
    if ( setsockopt(c_socket, SOL_SOCKET, SO_SNDTIMEO, &t, sizeof(t)) != 0 )
        throw NetworkException("Error setting send timeout value.", errno);
}

/**
 *  Sets the socket receive timeout.
 */
void Socket::setReceiveTimeOut(struct timeval t)
{
    if ( setsockopt(c_socket, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t)) != 0 )
        throw NetworkException("Error setting receive timeout value.", errno);
}

/**
 *  Returns the socket local address.
 */
IPAddr Socket::localAddr()
{
    sockaddr_in address;
    int len = sizeof(address);

    if (getsockname(c_socket, (sockaddr *) &address, (socklen_t *) &len) < 0) {
        throw NetworkException("Error getting local ip address.", errno);
    }

    return IPAddr(inet_ntoa(address.sin_addr));
}

/**
 *  Returns the socket local port.
 */
int Socket::localPort()
{
    sockaddr_in address;
    int len = sizeof(address);

    if (getsockname(c_socket, (sockaddr *) &address, (socklen_t *) &len) < 0) {
        throw NetworkException("Error getting local port.", errno);
    }

    return ntohs(address.sin_port);
}

string Socket::receive(int buffer_size)
{
    char* buffer = new char[buffer_size+1];
    int sz = recv(c_socket, buffer, sizeof(buffer), 0);

    buffer[sz] = '\0';
    string str(buffer);
    delete[] buffer;

    return str;
}

void Socket::setBlocking(bool blocking)
{
    int op = fcntl(c_socket, F_GETFL, 0);

    if(op == -1)
        throw NetworkException("Error changing socket blocking mode.", 0);

    if (blocking) {
        op &= ~O_NONBLOCK; 
    } else {
        op |= O_NONBLOCK;
    }

    if(fcntl(c_socket, F_SETFL, op) == -1)
    {
        throw NetworkException("Error changing socket blocking mode.", 0);
    }
}

// TO DO
// int Socket::select(vector<Socket> readSocket, vector<Socket> writeSocket, struct timeval time)
// {

// }

#endif
