#include <string.h>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>

#include "Error.hpp"

#ifndef IPADDR_H
#define IPADDR_H

typedef enum {IPV4 = AF_INET, IPV6 = AF_INET6, ANY = AF_UNSPEC} EFamily;

/** \brief Represents an IP Address.
 *
 *  
 */
class IPAddr
{

private:
    std::string strIpAddr;
    EFamily family;

public:
    IPAddr()
    {
        strIpAddr = "";
    }

    // Constructor
    IPAddr(std::string strIP)
    {
        struct addrinfo hint, *res = NULL;
        memset(&hint, '\0', sizeof hint);
        hint.ai_family = PF_UNSPEC;
        hint.ai_flags = AI_NUMERICHOST;

        int error_code = ::getaddrinfo(strIP.c_str(), NULL, &hint, &res);

        if (error_code) {
            std::string error_message("Invalid IP address: ");
            error_message += gai_strerror(error_code);

            throw std::invalid_argument(error_message);
        }

        // Test the IP's network
        switch(res->ai_family) {
            case AF_INET:
                family = IPV4;
                break;
            case AF_INET6:
                family = IPV6;
                break;
            default:
                throw NetworkException("Invalid IP type", 0);
        }

        strIpAddr = strIP;
        freeaddrinfo(res);
    }

    /** \brief Returns the EFamily (IPV4 or IPV6) of the IPAddress
      * 
      */
    EFamily getFamily()
    {
        return family;
    }

    /** \brief Generates a brodcast IP Address, given the mask.
      * Can only be called for IPV4 addresses.
      */
    IPAddr generateBroadcast(std::string mask)
    {
        if (family == IPV6) {
            throw std::invalid_argument("Broadcast doesn't work for IPV6 addresses.");
        }
        else {
            char dot;
            int ip_int[4] ;
            int mask_int[4] ;
            int broadcast_int[4];
            
            std::istringstream s1(strIpAddr);
            for (int i=0; i<4; i++)
                s1 >> ip_int[i] >> dot;

            std::istringstream s2(mask);
            for (int i=0; i<4; i++)
                s2 >> mask_int[i] >> dot;

            for (int i=0; i<4; i++)
                broadcast_int[i] = int(ip_int[i]) | int(mask_int[i]);

            std::ostringstream broadcast_s;
            broadcast_s << broadcast_int[0];
            for (int i=1; i<4; i++)
                broadcast_s << "." << broadcast_int[i];

            return IPAddr(broadcast_s.str());
        }
    }

    /** \brief Returns IP in string format.
      * 
      */
    std::string stringIP()
    {
        return strIpAddr;
    }

    /** \brief Check if the IP address is multicast.
      * 
      */
    bool isMulticast()
    {   
        std::istringstream s1(strIpAddr);
        if(family == IPV4) {
            
            int ip_int[4];
            char dot;
        
            s1 >> ip_int[0] >> dot >> ip_int[1] >> dot >> ip_int[2] >> dot >> 
            ip_int[3] >> dot;
        
            if(ip_int[0] >= 224 && ip_int[0] <= 239)
                return true;
            else
                return false;
        }else {
            throw std::invalid_argument("Multicast function doesn't work for IPV6 addresses.");
        }


    }
};

#endif