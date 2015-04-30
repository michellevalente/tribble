#include <DatagramSocket.hpp>

/** \brief Represents a multicast socket
 *  Allows the socket to join groups and set number of TTLs
 */
class Multicast : public DatagramSocket
{

public:
    Multicast();
    Multicast(uint16_t port);
    void joinGroup(IPAddr multicastAddr);
    void setTTL(unsigned char TTL);
};

Multicast::Multicast() : DatagramSocket()
{
    int multicastEnable = 1;

    // Allow multiple sockets to use the same port number
    if( setsockopt(c_socket,SOL_SOCKET,SO_REUSEADDR,&multicastEnable,sizeof(multicastEnable)) < 0)
        throw NetworkException("Could not open multicast socket.", errno);
}

Multicast::Multicast(uint16_t port) : DatagramSocket(port)
{
    int multicastEnable = 1;

    // Allow multiple sockets to use the same port number
    if( setsockopt(c_socket,SOL_SOCKET,SO_REUSEADDR,&multicastEnable,sizeof(multicastEnable)) < 0)
        throw NetworkException("Could not open multicast socket.", errno);
}

void Multicast::joinGroup(IPAddr multicastAddr)
{
	struct ip_mreq multicast;

	multicast.imr_interface.s_addr = htonl(INADDR_ANY);
	multicast.imr_multiaddr.s_addr = inet_addr(multicastAddr.stringIP().c_str());

	if(setsockopt(c_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
				  &multicast, sizeof(multicast)) < 0){
	   throw NetworkException("Join group error.", errno);
	}
		
}

void Multicast::setTTL(unsigned char TTL)
{
	if (setsockopt(c_socket, IPPROTO_IP, IP_MULTICAST_TTL, 
		           (void *) &TTL, sizeof(TTL)) < 0) 
    	throw NetworkException("Multicast TTL error", errno);
  
}

