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
		ipAddr = IPAddr("0.0.0.0");
		port = 0;
	};

	// Constructor
	HostAddr(std::string ip, uint16_t port_number = 0)
	{
		port = port_number;
		ipAddr = IPAddr(ip);
	}

	/** \brief Returns the socket IP in string format.
     */
	std::string getIp()
	{
		return ipAddr.stringIP();
	}

	/** \brief Returns the socket port number.
     */
	uint16_t getPort()
	{
		return port;
	}

	/** \brief Set the IP of the host address.
	 *	Receives the IP as a string.
     */
	void setIP(std::string ip)
	{
		ipAddr = IPAddr(ip);
	}

	/** \brief Sets the port number of the host address.
     */
	void setPort(uint16_t port_number)
	{
		port = port_number;
	}

	/** \brief Returns the IP address family.
     *  Receive the family as a EFamily.
     */
	EFamily getFamily()
	{
		return ipAddr.getFamily();
	}

};

#endif