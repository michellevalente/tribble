#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <iostream>

#include "HostAddr.hpp"


/* Sample program to test if it is a valid ip
 * and calculate broadcast ip. 				
 */
int main()
{
	string ip, mask, broadcastip;
	int family;
	
	cout << "Ip address: ";
	cin >> ip;

	HostAddr ipAddr(ip);

	family = ipAddr.family(); 

	if(family == 4)
		cout << ip << " is a valid IPV4 address." << endl;
	else
		cout << ip << " is a valid IPV4 address." << endl;

	cout << "Enter a mask to calculate the broadcast IP: ";
	cin >> mask;

	broadcastip = ipAddr.generateBroadcast(mask);
	cout << "Broadcast IP: " << broadcastip << endl;
}