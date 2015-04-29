#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <sstream>

using namespace std;

class ipAddr{

private:
	string ip_address;
	int type;

public:
	ipAddr(string ip)
	{

		struct addrinfo hint, *res = NULL;
	    int result;
	    memset(&hint, '\0', sizeof hint);
	    hint.ai_family = PF_UNSPEC;
	    hint.ai_flags = AI_NUMERICHOST;

	    result = getaddrinfo(ip.c_str(), NULL, &hint, &res);

	    if (result) {
	        throw std::invalid_argument("Invalid ip address");
	    }

	    /* Test for ipv4 address */
	    if(res->ai_family == AF_INET) {
	        type = 4;
	    } 
	    /* Test for ipv6 address */
	    else if (res->ai_family == AF_INET6) {
	        type = 6;
	    } 
	    else {
	        throw std::invalid_argument("Invalid ip address");
	    }
	   ip_address = ip;
	   freeaddrinfo(res);

	}

	/* Get version of ip. Returns 4 for IPV4 and 6 for IPV6 */
	int family()
	{
		return type;
	}

	ipAddr generateBroadcast(string mask)
	{
		if(type == 6)
			throw std::invalid_argument("Invalid ip address");
		else{
			char dot;
			int ip_int[4] ;
        	int mask_int[4] ;
        	int broadcast_int[4];
        	string broadcast_s;
			istringstream s1(ip_address);
			istringstream s2(mask);

			s1 >> ip_int[0] >> dot >> ip_int[1] >> dot >> ip_int[2] >> dot >> 
			ip_int[3] >> dot;

			s2 >> mask_int[0] >> dot >> mask_int[1] >> dot >> mask_int[2] >> dot >> 
			mask_int[3] >> dot;

			for(int i = 0; i < 4 ; i++){
				broadcast_int[i] = int(ip_int[i]) | int(mask_int[i]) ;
			}

			broadcast_s = to_string(broadcast_int[0]) + "." + to_string(broadcast_int[1]) + 
			"." + to_string(broadcast_int[2]) + "." + to_string(broadcast_int[3]);

			ipAddr broadcast(broadcast_s);

			return broadcast;
		}
	}

	string toString()
	{
		return ip_address;
	}
};