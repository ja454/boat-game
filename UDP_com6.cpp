
#include <iostream>
#include <cstdio>
#include <cmath> 
#include <fstream> 

#include <Ws2tcpip.h>
#include <WinSock2.h>

#include "UDP_com.h"
#include "UDP_com6.h"

using namespace std;

int activate_socket6(int port, char *local_IP_address, int &sock)
// local_IP_address -- use NULL to accept messages for all interfaces
{
    struct sockaddr_in6 local_addr; // Local address
    unsigned short UDP_port;     // UDP port
    unsigned long nonblocking;   // Flag to make socket nonblocking
	int IPv6_only;

	// set port number
    UDP_port = (unsigned short)port;

	// Create socket for sending / receiving datagrams
	sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if ( sock == INVALID_SOCKET ) {
		cout << "\nsocket() function error";
		return 1;
	}

	// All SOL_SOCKET socket options apply equally to IPv4 and IPv6 
	// (except SO_BROADCAST, since broadcast is not implemented in IPv6).

    // construct local address structure
    memset(&local_addr, 0, sizeof(local_addr));   // Zero out structure
    local_addr.sin6_family = AF_INET6;                // Internet address family    
	local_addr.sin6_port = htons(UDP_port);      // Local port

	// set which IP address/interface (NIC card) messages can be received from
	if ( local_IP_address != NULL ) { 
		// accept only the indicated interface
		inet_pton(AF_INET6,local_IP_address,&(local_addr.sin6_addr));
	} else { 
		// use any incoming IPv6 interface
		local_addr.sin6_addr = in6addr_any;
	}

	// use socket for IPv6 only (no IPv4 communication)
	IPv6_only = 1;
	if( setsockopt( sock, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&IPv6_only, 
		sizeof(IPv6_only) ) ) {
		cout << "\nsetsockopt function error: IPV6_V6ONLY";
		return 1;
	}

	// set the socket to nonblocking I/O
	nonblocking = 1;
    if ( ioctlsocket(sock, FIONBIO, &nonblocking) ) {
		cout << "\nioctlsocket() function error";
		return 1;
	}

    // bind the socket to the local interface address
    if ( bind( sock, (struct sockaddr *) &local_addr, sizeof(local_addr) ) ) {
		cout << "\nbind() function error";
		return 1;
	}

	return 0;
}


int deactivate_socket6(int &sock)
{
	closesocket(sock);

	return 0;
}


int recv6(char *buffer, int &size, char *IP_address, int sock)
{
    int addr_len, error;
    struct sockaddr_in6 recv_addr; // IP address of received message

	// set the size of the in-out parameter
	addr_len = sizeof(recv_addr);

	size = recvfrom(sock, buffer, NMAX_UDP_BUFFER, 0,
					(struct sockaddr *) &recv_addr, &addr_len);

	if ( size < 0 ) {

		error = WSAGetLastError(); // read error code

		if ( (error == WSAEWOULDBLOCK) || (error == WSAECONNRESET) ) { 
			// no mesage received yet
			size = -1;
			return size; 
		} else { // error
			cout << "\nrecvfrom() error , code = " << error;
			size = -2;
			return size;
		}

	}

	// get IP address of message
	inet_ntop(AF_INET6,&(recv_addr.sin6_addr),IP_address,NMAX_ADDRESS);

	return size;
}


int send6(char *buffer, int size, char *IP_address, int sock, int port)
{
    struct sockaddr_in6 send_addr; // IP address of send message
    unsigned short UDP_port;     // UDP port

	// set port number
    UDP_port = (unsigned short)port;

    // construct send address structure
    memset(&send_addr, 0, sizeof(send_addr));   // Zero out structure
    send_addr.sin6_family = AF_INET6;           // Internet address family    
	send_addr.sin6_port = htons(UDP_port);      // Local port
	inet_pton(AF_INET6,IP_address,&(send_addr.sin6_addr));

	// Send received datagram back to the client
	if ( sendto(sock, buffer, size, 0, (struct sockaddr *) &send_addr, 
		sizeof(send_addr)) != size ) {

		cout << "\nsendto() sent a different number of bytes than expected";

		return 1;
	}

	return 0;
}

