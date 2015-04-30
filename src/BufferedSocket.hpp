#include <system_error>
#include <queue>
#include "Socket.hpp"
#include "DNS.hpp"


#ifndef BUFFERED_SOCKET_H
#define BUFFERED_SOCKET_H

class ServerSocket;

class BufferedSocket: public Socket
{

    friend class ServerSocket;

public:
    BufferedSocket();
    string getPeerAddress() const;
    unsigned short getPeerPort() const;
    BufferedSocket& operator >>( std::string& s );
    void sendAll(const string& to_send) const;
    BufferedSocket& operator << ( const std::string& s );
    BufferedSocket& operator<<( string(*fptr)(void) );
    void flush();
    void fillBuffer();
    string getNextString();
    BufferedSocket& getNextLine(std::string& s);
    void connect(string server, unsigned short port);

    void setC_Socket(int s) // @TODO: check if this is ok
    {
        c_socket=s;
    }

    operator bool()
    {
        return !eof;
    }


protected:
    std::queue<char> receivedBuffer;
    std::string toSend;
    bool eof;
};


void BufferedSocket::sendAll(const string& to_send) const
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


BufferedSocket::BufferedSocket()
{
    eof=false;

    if ((c_socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw NetworkException("Socket creation failed.", errno);
}

string BufferedSocket::getPeerAddress() const
{
    sockaddr addr;
    unsigned int addrLen = sizeof(addr);

    if(getpeername(c_socket, &addr, &addrLen) < 0)
    {
        throw NetworkException("Getting address failed.", errno);
    }

    return string(inet_ntoa(((sockaddr_in*) &addr)->sin_addr));
}

uint16_t BufferedSocket::getPeerPort() const
{
    sockaddr addr;
    unsigned int addrLen = sizeof(addr);

    if(getpeername(c_socket, &addr, &addrLen) < 0)
    {
        throw NetworkException("Getting address failed.", errno);
    }

    return ((sockaddr_in*) &addr)->sin_port;
}


BufferedSocket& BufferedSocket::operator<<(const std::string& s)
{
    toSend+=s;
    return *this;
}


string endN()
{
    return "\n";
}

string endRN()
{
    return "\r\n";
}

BufferedSocket& BufferedSocket::operator<<( string(*fptr)(void) )
{
    toSend+=fptr();
    return *this;
}


void BufferedSocket::flush()
{
    sendAll(toSend);
    toSend.clear();
}

#define FILLING_SIZE 100

void BufferedSocket::fillBuffer()
{
    char buffer[FILLING_SIZE];
    int succesfullyReceived = recv( c_socket, buffer, FILLING_SIZE, 0 );
    if(succesfullyReceived > 0)
    {
        for(int i=0;i<succesfullyReceived;i++)
            receivedBuffer.push(buffer[i]);
    }
    else if(succesfullyReceived==0)
    {
        eof=true; // client has closed the connection
    }
    else
    {
        int _errno = errno;
        throw NetworkException("Receiving from socket failed", _errno);
    }
}

string BufferedSocket::getNextString()
{
    string nextString = "";
    if(receivedBuffer.size()==0) fillBuffer();

    // discarding any delimitors
    while( !eof && ( receivedBuffer.front()=='\n' || receivedBuffer.front()=='\r' || receivedBuffer.front()==' ' ) )
    {
        receivedBuffer.pop();
        if(receivedBuffer.size()==0) fillBuffer();
    }

    // inserting in nextString
    while( !eof && ( receivedBuffer.front()!='\n' && receivedBuffer.front()!='\r' && receivedBuffer.front()!=' ' ) )
    {
        nextString+=receivedBuffer.front();
        receivedBuffer.pop();
        if(receivedBuffer.size()==0) fillBuffer();
    }

    // discard carriage, if any
    if( !eof && (receivedBuffer.front()=='\r') )
    {
        receivedBuffer.pop();
        //if(receivedBuffer.size()==0) fillBuffer();
    }

    // discard new line, if any
    if( !eof && (receivedBuffer.front()=='\n') )
    {
        receivedBuffer.pop();
        //if(receivedBuffer.size()==0) fillBuffer();
    }

    return nextString;
}

BufferedSocket& BufferedSocket::operator>>(std::string& s)
{
    s = getNextString();
    return *this;
}


BufferedSocket& BufferedSocket::getNextLine(std::string& s)
{
    string nextLine = "";
    if(receivedBuffer.size()==0) fillBuffer();

    // inserting in nextLine
    while( !eof && ( receivedBuffer.front()!='\n' && receivedBuffer.front()!='\r' ) ) 
    {                                                                                    
        nextLine+=receivedBuffer.front();
        receivedBuffer.pop();
        if(receivedBuffer.size()==0) fillBuffer();
    }

    // discarding carriage demilitors
    while( !eof && receivedBuffer.front()=='\r' )
    {
        receivedBuffer.pop();
        if(receivedBuffer.size()==0) fillBuffer();
    }

    // discarding at most one \n
    if( !eof && receivedBuffer.front()=='\n')
    {
        receivedBuffer.pop();
        //if(receivedBuffer.size()==0) fillBuffer();
    }

    s = nextLine;

    return *this;
}

void BufferedSocket::connect(string server, unsigned short port)
{
    std::vector<IPAddr> ips = dns_lookup( server , IPV4);

    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr( ips[0].stringIP().c_str() );
    servAddr.sin_port = htons(port); 

    if( ::connect(c_socket, (sockaddr *) &servAddr, sizeof(servAddr) ) < 0 )
        throw NetworkException("Connecting BufferedSocket failed.",0);
}



#endif
