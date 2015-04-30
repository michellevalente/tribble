#include "Socket.hpp"

#ifndef STREAM_SOCKET_H
#define STREAM_SOCKET_H

class StreamSocket: public Socket
{

public:
    StreamSocket();
    StreamSocket(uint16_t port_number);
    StreamSocket accept();
    void send_all(const string& to_send) const;
    string getPeerAddress() const;
    unsigned short getPeerPort() const;
    int operator >>( std::string& s ) const;
    const StreamSocket& operator << ( const std::string& s ) const;
};


void StreamSocket::send_all(const string& to_send) const{
    int length = to_send.length();
    const char *ptr = &to_send[0];
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

StreamSocket::StreamSocket(uint16_t port) {
    this->port_number = port;

    if ((c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw NetworkException("Socket error.");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));       /* Zero out structure */
    addr.sin_family = AF_INET;                /* Internet address family */
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    addr.sin_port = htons(this->port_number);              /* Local port */

    /* Bind to the local address */
    ::bind(c_socket, (struct sockaddr *)&addr, sizeof(addr));  // make error

    /* Mark the socket so it will listen for incoming connections */
    if (listen(c_socket, MAXPENDING) < 0)
        throw std::invalid_argument("Listen error.");
}


string StreamSocket::getPeerAddress() const
{
    sockaddr addr;
    unsigned int addrLen = sizeof(addr);

    if(getpeername(c_socket, &addr, &addrLen) < 0)
    {
        throw NetworkException("Could not get peer name.");
    }

    return string(inet_ntoa(((sockaddr_in*) &addr)->sin_addr));
}

uint16_t StreamSocket::getPeerPort() const
{
    sockaddr addr;
    unsigned int addrLen = sizeof(addr);

    if(getpeername(c_socket, &addr, &addrLen) < 0)
    {
        throw NetworkException("Could not get peer name.");
    }

    return ((sockaddr_in*) &addr)->sin_port;
}

const StreamSocket& StreamSocket::operator << ( const std::string& s ) const
{
    send_all(s);
    return *this;
}

#define CHUNK_SIZE 10

int StreamSocket::operator >>( std::string& s ) const
{
    char *chunking = new char[CHUNK_SIZE];
    int chunked;

    s = "";

    do
    {
        chunked = recv( c_socket, chunking, CHUNK_SIZE, 0 );

        if(chunked<0)
        {
            delete [] chunking;
            throw NetworkException("Could not hear from socket.");
        }
        if(chunked>0)
        {
            s.insert( s.size() , chunking , chunked );
        }

        //cout<<"recv()ed chunked: "<<chunked<<" as of now: "<<s<<endl;

    }
    while(chunked > 0); // zero indicates end of connection (ctrl-D)


    delete [] chunking;
    return s.size();
}

#endif
