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
#include "error.hpp"
#include "hostAddr.hpp"

using namespace std;

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

    string receive(int buffer_size, error_code error);
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

class StreamSocket: public Socket
{

public:
    StreamSocket();
    StreamSocket(int port_number);
    StreamSocket accept();
    void send_all(string to_send);
};

class DatagramSocket: public Socket
{

public:
    DatagramSocket();
    DatagramSocket(int port_number);
    string receiveFrom(int buffer_size, HostAddr& client);
    void sendTo(string message, HostAddr &client);
};

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

string Socket::receive(int buffer_size, error_code error) {
    char* buffer = new char[buffer_size+1];
    int sz = recv(c_socket, buffer, sizeof(buffer), 0);

    buffer[sz] = '\0';
    string str(buffer);
    delete[] buffer;

    return str;
}

void StreamSocket::send_all(string to_send) {
    int length = to_send.length();
    char *ptr = &to_send[0];
    while (length > 0)
    {
        int i = send(c_socket, ptr, length, 0);
        if (i < 1) 
            throw NetworkException("Send message error.");
        ptr += i;
        length -= i;
    }
}

StreamSocket StreamSocket::accept() {
    StreamSocket sock;
    struct sockaddr_in addr;
    unsigned int addrLen = sizeof(addr); 
    sock.c_socket = ::accept(c_socket, (struct sockaddr *)&addr, &addrLen);

    return sock;
}

StreamSocket::StreamSocket() {
    if ((c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw NetworkException("Socket error.");
}

StreamSocket::StreamSocket(int port_number) {
    this->port_number = port_number;

    if ((c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw NetworkException("Socket error.");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));       /* Zero out structure */
    addr.sin_family = AF_INET;                /* Internet address family */
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    addr.sin_port = htons(port_number);              /* Local port */

    /* Bind to the local address */
    bind(c_socket, (struct sockaddr *)&addr, sizeof(addr));  // make error

    /* Mark the socket so it will listen for incoming connections */
    if (listen(c_socket, MAXPENDING) < 0)
        throw std::invalid_argument("Listen error.");
}

DatagramSocket::DatagramSocket() {
    if ((c_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        throw std::invalid_argument("Socket error.");
}  

DatagramSocket::DatagramSocket(int port_number) {
    this->port_number = port_number;

    if ((c_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        int tmp = errno;
        printf("ERROR: %s , errno %d\n", strerror(tmp), tmp);
        throw NetworkException("Socket error.");
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));       /* Zero out structure */
    addr.sin_family = AF_INET;                /* Internet address family */
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    addr.sin_port = htons(port_number);              /* Local port */

    /* Bind to the local address */
    int tmp = ::bind(c_socket, (struct sockaddr *)&addr, sizeof(addr)); // make error
    // int tmp = errno;
    if (tmp < 0) {
        printf("ERROR: %s , errno %d\n", strerror(tmp), tmp);
        throw NetworkException("Socket error.");
    }

}

string DatagramSocket::receiveFrom(int buffer_size, HostAddr &client){
    struct sockaddr_in clientAddr;
    socklen_t len;
    char * addr;
    char* buffer = new char[buffer_size+1];

    int sz = recvfrom(c_socket, buffer, buffer_size, 0,(struct sockaddr *)&clientAddr, &len);
    int tmp = errno;
    if (tmp < 0)
        printf("ERROR: %s , errno %d\n", strerror(tmp), tmp);


    buffer[sz] = '\0';
    string str(buffer);
    delete[] buffer;

    client.setPort(clientAddr.sin_port);
    addr = inet_ntoa(clientAddr.sin_addr);
    client.setIp(string(addr));

    return str;
}

void DatagramSocket::sendTo(string message, HostAddr &client){
    int length = message.length();
    char *ptr = &message[0];
    struct sockaddr_in clientAddr;

    /* Set up client */
    if(client.family() == 6)
        clientAddr.sin_family = AF_INET6;
    else
        clientAddr.sin_family = AF_INET;


    clientAddr.sin_port = htons(client.getPort());

    inet_aton(client.getIp().c_str(), &clientAddr.sin_addr);

    /* Send message */
    while (length > 0)
    {
        int i = sendto(c_socket,ptr,length,0,(struct sockaddr *)&clientAddr,sizeof(clientAddr));
        if (i < 1) 
            throw NetworkException("Send message error.");
        ptr += i;
        length -= i;
    }
}
