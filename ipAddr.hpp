#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>

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
	    if(res->ai_family == AF_INET) {
	        type = 4;
	    } else if (res->ai_family == AF_INET6) {
	        type = 6;
	    } else {
	        throw std::invalid_argument("Invalid ip address");
	    }

	   freeaddrinfo(res);

	}
};