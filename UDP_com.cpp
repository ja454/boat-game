
#include <iostream>
#include <cstdio>
#include <cmath> 
#include <fstream> 

#include <Ws2tcpip.h>
#include <WinSock2.h>

#include "UDP_com.h"

using namespace std;

int activate_network()
{
	WORD wVersionRequested;	// Version of Winsock to load
    WSADATA wsaData;		// Winsock implementation details

	wVersionRequested = MAKEWORD(2,0);	// Request Winsock v2.0

	if( WSAStartup(wVersionRequested, &wsaData) ) { // Load Winsock DLL
		cout << "\nWSAStartup() error";
		return 1;
	}

	return 0;
}


int deactivate_network()
{
	WSACleanup();

	return 0;
}


int activate_socket(int port, char *local_IP_address, int &sock, int mode)
// local_IP_address -- use NULL to accept messages for all interfaces
// mode = 0 -- transmit to only one computer (unicast)
// mode = 1 -- transmit to all computers on local network (broadcast)
{
    struct sockaddr_in local_addr; // Local address
    unsigned short UDP_port;     // UDP port
    unsigned long nonblocking;   // Flag to make socket nonblocking
    int broadcastPermission;         // Socket opt to set permission to broadcast

	// set port number
    UDP_port = (unsigned short)port;

	// Create socket for sending / receiving datagrams
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if ( sock == INVALID_SOCKET ) {
		cout << "\nsocket() function error";
		return 1;
	}

	// note mode == 1 only works for IPv4 (not IPv6)
	if ( mode == 1 ) { // set broadcast mode (unicast is default)
		broadcastPermission = 1;
		if ( setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&broadcastPermission, 
			sizeof(broadcastPermission) ) ) {
			cout << "\nsetsockopt() function error: SO_BROADCAST";
			return 1;
		}
	}

    // construct local address structure
    memset(&local_addr, 0, sizeof(local_addr));   // Zero out structure
    local_addr.sin_family = AF_INET;                // Internet address family    
	local_addr.sin_port = htons(UDP_port);      // Local port

	// set which IP address/interface (NIC card) messages can be received from
	if ( local_IP_address != NULL ) { 
		// accept only the indicated interface
	    local_addr.sin_addr.s_addr = inet_addr(local_IP_address);
	} else { 
		// use any incoming interface
		local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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


int deactivate_socket(int &sock)
{
	closesocket(sock);

	return 0;
}


int recv(char *buffer, int &size, char *IP_address, int sock)
{
    int addr_len, error;
    struct sockaddr_in recv_addr; // IP address of received message

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
	strcpy(IP_address,inet_ntoa(recv_addr.sin_addr));

	return size;
}


int send(char *buffer, int size, char *IP_address, int sock, int port)
{
    struct sockaddr_in send_addr; // IP address of send message
    unsigned short UDP_port;     // UDP port

	// set port number
    UDP_port = (unsigned short)port;

    // construct send address structure
    memset(&send_addr, 0, sizeof(send_addr));   // Zero out structure
    send_addr.sin_family = AF_INET;                // Internet address family    
	send_addr.sin_port = htons(UDP_port);      // Local port
	send_addr.sin_addr.s_addr = inet_addr(IP_address);

	// Send received datagram back to the client
	if ( sendto(sock, buffer, size, 0, (struct sockaddr *) &send_addr, 
		sizeof(send_addr)) != size ) {

		cout << "\nsendto() sent a different number of bytes than expected";

		return 1;
	}

	return 0;
}

