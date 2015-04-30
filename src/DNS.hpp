#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <netdb.h>      /* for gethostbyname() */
#include <unistd.h>     /* for close() */
#include <signal.h>     /* for signal() */
#include <sys/stat.h>   /* for stat() */
#include <string.h>     /* for memset() */
#include <vector>
#include <string>
#include <system_error>
#include "IPAddr.hpp"

#ifndef DNS_H
#define DNS_H

using namespace std;

/* Function to translate hostname to ip address. Default ip_version returns 
 * ipv4 and ipv6 addresses */
vector<IPAddr> dns_lookup(const string hostname, EFamily version_ip = ANY){

    vector<IPAddr> ip; 
    struct addrinfo hints;
    struct addrinfo *result, *it;
    int s;

    memset(&hints, 0, sizeof(struct addrinfo));

    if(version_ip == IPV6)
        hints.ai_family = AF_INET6;
    else if(version_ip == IPV4)
        hints.ai_family = AF_INET;
    else
        hints.ai_family = AF_UNSPEC;

    hints.ai_socktype = SOCK_STREAM;

    s = getaddrinfo(hostname.c_str(), NULL, &hints, &result);

    if (s != 0) {
         return ip;
    }

    for(it = result;it != NULL; it = it->ai_next) {

        void * ip_address;
        struct sockaddr_in * ipv4;
        struct sockaddr_in6 *ipv6;
        char ip_string[INET6_ADDRSTRLEN];

        // IPV4
        if (it->ai_family == AF_INET) { 
            ipv4 = (struct sockaddr_in *) it->ai_addr;
            ip_address = &(ipv4->sin_addr);
        } 
        // IPV6
        else { 
            ipv6 = (struct sockaddr_in6 *) it->ai_addr;
            ip_address = &(ipv6->sin6_addr);
        }
 
        // convert the IP to a string
        inet_ntop(it->ai_family, ip_address, ip_string, sizeof (ip_string));
        IPAddr ipAddress(ip_string);

        // Put string in the vector
        ip.push_back(ipAddress);
    }

    return ip;

}

#endif