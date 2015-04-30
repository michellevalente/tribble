#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <sstream>
#include <string.h>
#include "Error.hpp"
#include "IPAddr.hpp"

#ifndef HOSTADDR_H
#define HOSTADDR_H

/** \brief Represents a socket address.
 *
 *  
 */
class HostAddr{

private:
	IPAddr ip_address;
	uint16_t port;

public:

	HostAddr(){
		ip_address = IPAddr("");
		port = 0;
	};

	HostAddr(std::string ip, uint16_t port_number = 0)
	{
		port = port_number;
		ip_address = IPAddr(ip);
	}

	std::string getIp()
	{
		return ip_address.stringIP();
	}

	uint16_t getPort()
	{
		return port;
	}

	void setIP(std::string ip)
	{
		ip_address = IPAddr(ip);
	}

	void setIP(IPAddr ip)
	{
		ip_address = ip;
	}

	void setPort(uint16_t port_number)
	{
		port = port_number;
	}

	EFamily getFamily()
	{
		return ip_address.getFamily();
	}

};

#endif