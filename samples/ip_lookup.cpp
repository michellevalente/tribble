#include <iostream>
#include "DNS.hpp"

int main()
{
	string hostname;
	vector<IPAddr> ip;

	cout << "hostname: ";
	cin >> hostname;

	cout << "IPV4: \n";

	ip = dns_lookup(hostname, 4);

	for(int i = 0; i < ip.size(); i++){
		cout << " " << ip[i].stringIP() << endl;
	}

	cout << "IPV6: \n";

	ip = dns_lookup(hostname, 6);
	
	for(int i = 0; i < ip.size(); i++){
		cout << " " << ip[i].stringIP() << endl;
	}

}
