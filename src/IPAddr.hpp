#include <string.h>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "Error.hpp"

#ifndef IPADDR_H
#define IPADDR_H

typedef enum {IPV4 = AF_INET, IPV6 = AF_INET6} EFamily;

/** \brief Represents an IP Address.
 *
 *  
 */
class IPAddr
{

private:
	std::string ip_address;
	EFamily family;

public:
	IPAddr()
	{
		ip_address = "";
	}
	IPAddr(std::string ip)
	{
		struct addrinfo hint, *res = NULL;
	    int result;
	    memset(&hint, '\0', sizeof hint);
	    hint.ai_family = PF_UNSPEC;
	    hint.ai_flags = AI_NUMERICHOST;

	    result = getaddrinfo(ip.c_str(), NULL, &hint, &res);

	    if (result) {
	        throw NetworkException("Invalid IP address");
	    }

	    // Test for ipv4 address 
	    if(res->ai_family == AF_INET) {
	        family = IPV4;
	        
	    } 
	    // Test for ipv6 address 
	    else if (res->ai_family == AF_INET6) {
	        family = IPV6;
	    } 
	    else {
	        throw NetworkException("Invalid IP address");
	    }

	    ip_address = ip;
	    freeaddrinfo(res);
	}

	//  Returns 4 for IPV4 and 6 for IPV6 
	EFamily getFamily()
	{
		return family;
	}

	IPAddr generateBroadcast(std::string mask)
	{
		if(family == IPV6)
			throw NetworkException("Invalid ip address");
		else{
			char dot;
			int ip_int[4] ;
        	int mask_int[4] ;
        	int broadcast_int[4];
        	std::string broadcast_s;
			std::istringstream s1(ip_address);
			std::istringstream s2(mask);

			s1 >> ip_int[0] >> dot >> ip_int[1] >> dot >> ip_int[2] >> dot >> 
			ip_int[3] >> dot;

			s2 >> mask_int[0] >> dot >> mask_int[1] >> dot >> mask_int[2] >> dot >> 
			mask_int[3] >> dot;

			for(int i = 0; i < 4 ; i++){
				broadcast_int[i] = int(ip_int[i]) | int(mask_int[i]) ;
			}

			broadcast_s = std::to_string(broadcast_int[0]) + "." + std::to_string(broadcast_int[1]) + 
			"." + std::to_string(broadcast_int[2]) + "." + std::to_string(broadcast_int[3]);

			return IPAddr(broadcast_s);
		}
	}

	std::string stringIP()
	{
		return ip_address;
	}
};

#endif