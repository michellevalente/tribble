#include "DatagramSocket.hpp"


#ifndef BROADCAST_H
#define BROADCAST_H

class Broadcast : public DatagramSocket
{

public:
    Broadcast();
    void sendTo(string message, HostAddr &client);

};

Broadcast::Broadcast() : DatagramSocket()
{
    int broadcastEnable = 1;
    if( setsockopt(c_socket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)))
        throw NetworkException("Could not open broadcast socket.");
}

void Broadcast::sendTo(string message, HostAddr &client)
{
    int length = message.length();
    char *ptr = &message[0];
    sockaddr_in broadcast_addr;
    int len;

    if(client.getFamily() == 6)
        broadcast_addr.sin_family = AF_INET6;
    else
        broadcast_addr.sin_family = AF_INET;

    broadcast_addr.sin_port = htons(client.getPort());
    broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;
    len = sizeof(broadcast_addr);

    /* Send message */
    while (length > 0)
    {
        int i = sendto(c_socket,ptr,length,0,(struct sockaddr *)&broadcast_addr,sizeof(broadcast_addr));
        int tmp = errno;
        if(i < 0) {
            printf("ERROR: %s , errno %d\n", strerror(tmp), tmp);
            printf("while sending to port %d\n", client.getPort());
            throw NetworkException("Send message error.");
        }
        
        ptr += i;
        length -= i;
    }

}

#endif
