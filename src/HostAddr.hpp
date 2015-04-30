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

/** \brief Represents an endpoint address (IP, Port)
 *	Port is stored as a uint16, ip address is an IPAddr
 */
class HostAddr
{
private:
	IPAddr ipAddr;
	uint16_t port;

public:

	HostAddr(){
		ipAddr = IPAddr("");
		port = 0;
	};

	HostAddr(std::string ip, uint16_t port_number = 0)
	{
		port = port_number;
		ipAddr = IPAddr(ip);
	}

	std::string getIp()
	{
		return ipAddr.stringIP();
	}

	uint16_t getPort()
	{
		return port;
	}

	void setIP(std::string ip)
	{
		ipAddr = IPAddr(ip);
	}

	void setPort(uint16_t port_number)
	{
		port = port_number;
	}

	EFamily getFamily()
	{
		return ipAddr.getFamily();
	}

};

#endif