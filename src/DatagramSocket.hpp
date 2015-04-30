#include "Socket.hpp"

#ifndef DATAGRAM_SOCKET_H
#define DATAGRAM_SOCKET_H

/** \brief Represents a Datagram Socket (UDP)
 *  
 */
class DatagramSocket: public Socket
{

public:
    DatagramSocket();
    DatagramSocket(uint16_t port_number);
    string receiveFrom(int buffer_size, HostAddr& client);
    void sendTo(string message, HostAddr &client);
};

DatagramSocket::DatagramSocket() 
{
    if ((c_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        throw NetworkException("Socket creation failed", errno);
}  

DatagramSocket::DatagramSocket(uint16_t port_number)
{
    if ((c_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        throw NetworkException("Socket creation failed", errno);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));           /* Zero out structure */
    addr.sin_family = AF_INET;                /* Internet address family */
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    addr.sin_port = htons(port_number);       /* Local port */

    /* Bind to the local address */
    if (::bind(c_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        throw NetworkException("Socket creation failed", errno);
    }
}

string DatagramSocket::receiveFrom(int buffer_size, HostAddr &client)
{
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);
    char* addr;
    char* buffer = new char[buffer_size+1];

    int sz = ::recvfrom(c_socket, buffer, buffer_size, 0, (struct sockaddr *)&clientAddr, &len);
    //TODO: Handle possible errors

    buffer[sz] = '\0';
    string str(buffer);
    delete[] buffer;

    client.setPort(ntohs(clientAddr.sin_port));
    addr = inet_ntoa(clientAddr.sin_addr);
    client.setIP(string(addr));

    return str;
}

void DatagramSocket::sendTo(string message, HostAddr &client)
{
    int length = message.length();
    char *ptr = &message[0];
    struct sockaddr_in clientAddr;

    clientAddr.sin_family = client.getFamily();

    clientAddr.sin_port = htons(client.getPort());
    inet_aton(client.getIp().c_str(), &clientAddr.sin_addr);

    while (length > 0) {
        int i = ::sendto(c_socket, ptr, length, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
        int _errno = errno;
        if(i < 0) {
            throw NetworkException("Sending through socket failed", errno);
        }

        ptr += i;
        length -= i;
    }
}

#endif