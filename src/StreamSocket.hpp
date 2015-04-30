#include <system_error>
#include "Socket.hpp"

#ifndef STREAM_SOCKET_H
#define STREAM_SOCKET_H

/** \brief Represents a Stream Socket (TCP)
 */
class StreamSocket: public Socket
{

public:
    StreamSocket();
    void connect(HostAddr& peer);
    void send_all(const string& to_send) const;
    void send(const string& to_send, int buffer_size);
    string getPeerAddress() const;
    unsigned short getPeerPort() const;
    int operator >>( std::string& s ) const;
    const StreamSocket& operator << ( const std::string& s ) const;
    void changeSocket(int sock);
};

/** \brief Sends the entire string.
 */
void StreamSocket::send_all(const string& to_send) const
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

/** \brief Sends part of the string(buffer size).
 */
void StreamSocket::send(const string& to_send, int buffer_size)
{
    const char * ptr = &to_send[0];
    int sz = ::send(c_socket, ptr, buffer_size, 0);
    if(sz < 1)
        throw NetworkException("Error sending message.", errno);
}

StreamSocket::StreamSocket()
{
    if ((c_socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw NetworkException("Socket creation failed.", errno);
}

/** \brief Accepts a new connection.
 */
void StreamSocket::connect(HostAddr& peer)
{
    struct sockaddr_in peerAddr;

    memset(&peerAddr, 0, sizeof(peerAddr));     /* Zero out structure */
    peerAddr.sin_family      = AF_INET;             /* Internet address family */
    peerAddr.sin_addr.s_addr = inet_addr(peer.getIp().c_str());   /* Server IP address */
    peerAddr.sin_port        = htons(peer.getPort()); /* Server port */

    if (::connect(c_socket, (struct sockaddr *) &peerAddr, sizeof(peerAddr)) < 0)
    {
        throw NetworkException("Connecting to socket failed.", 0);
    }
        

}

/** \brief Returns the peer's IP.
 */
string StreamSocket::getPeerAddress() const
{
    sockaddr addr;
    unsigned int addrLen = sizeof(addr);

    if(getpeername(c_socket, &addr, &addrLen) < 0)
    {
        throw NetworkException("Getting address failed.", errno);
    }

    return string(inet_ntoa(((sockaddr_in*) &addr)->sin_addr));
}

/** \brief Returns the peer's port number.
 */
uint16_t StreamSocket::getPeerPort() const
{
    sockaddr addr;
    unsigned int addrLen = sizeof(addr);

    if(getpeername(c_socket, &addr, &addrLen) < 0)
    {
        throw NetworkException("Getting address failed.", errno);
    }

    return ((sockaddr_in*) &addr)->sin_port;
}

const StreamSocket& StreamSocket::operator<<(const std::string& s) const
{
    send_all(s);
    return *this;
}

#define CHUNK_SIZE 10

int StreamSocket::operator>>(std::string& s) const
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

void StreamSocket::changeSocket(int sock)
{
    c_socket = sock;
}

#endif
