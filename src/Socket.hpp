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
#include "Error.hpp"
#include "HostAddr.hpp"

using namespace std;

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
    Socket(){
        MAXPENDING = 10;
    }

    ~Socket()
    {
        if(close(c_socket)!=0) throw NetworkException("Socket could not be closed.");
    }

    string receive(int buffer_size);
    void SendSize(int bytes);
    void ReceiveSize(int bytes);
    int getSendSize();
    int getReceiveSize();
    void SendTimeOut(struct timeval time);
    void ReceiveTimeOut(struct timeval time);
    string localAddr();
    //string peerAddr();
    int localPort();
    
};

// class Multicast : public DatagramSocket
// {

// private:
//     HostAddr addr;
// public:
//     Multicast(uint16_t port_number);

// };

void Socket::SendSize(int bytes){

    if ( setsockopt(c_socket, SOL_SOCKET, SO_SNDBUF, &bytes, sizeof(bytes)) != 0 )
        throw NetworkException("Send Buffer error");
}

void Socket::ReceiveSize(int bytes){

    if ( setsockopt(c_socket, SOL_SOCKET, SO_RCVBUF, &bytes, sizeof(bytes)) != 0 )
        throw NetworkException("Receive Buffer error");
}

int Socket::getSendSize(){
    int size;
    socklen_t len=sizeof(size);
    if ( getsockopt(c_socket, SOL_SOCKET, SO_SNDBUF, &size, &len) != 0 )
        throw NetworkException("Send Buffer error");

    return size;
}

int Socket::getReceiveSize(){
    int size;
    socklen_t len=sizeof(size);
    if ( getsockopt(c_socket, SOL_SOCKET, SO_RCVBUF, &size, &len) != 0 )
        throw NetworkException("Send Buffer error");

    return size;
}

void Socket::SendTimeOut(struct timeval t){
    if ( setsockopt(c_socket, SOL_SOCKET, SO_SNDTIMEO, &t, sizeof(t)) != 0 )
        throw NetworkException("Send Timeout error");
}

void Socket::ReceiveTimeOut(struct timeval t){
    if ( setsockopt(c_socket, SOL_SOCKET, SO_RCVBUF, &t, sizeof(t)) != 0 )
        throw NetworkException("Receive Timeout error");
}

string Socket::localAddr(){
    sockaddr_in address;
    int len = sizeof(address);

    if (getsockname(c_socket, (sockaddr *) &address, (socklen_t *) &len) < 0) {
        throw NetworkException("Get Local Address failed.");
    }

    return inet_ntoa(address.sin_addr);
}

int Socket::localPort(){
    sockaddr_in address;
    int len = sizeof(address);

    if (getsockname(c_socket, (sockaddr *) &address, (socklen_t *) &len) < 0) {
        throw NetworkException("Get Local Port failed.");
    }

    return ntohs(address.sin_port);
}

string Socket::receive(int buffer_size) {
    char* buffer = new char[buffer_size+1];
    int sz = recv(c_socket, buffer, sizeof(buffer), 0);

    buffer[sz] = '\0';
    string str(buffer);
    delete[] buffer;

    return str;
}



// Multicast::Multicast(uint16_t port_number) : DatagramSocket(port_number)
// {
//     int broadcast_on = 1;
//     setsockopt(c_socket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes));
// }

#endif
