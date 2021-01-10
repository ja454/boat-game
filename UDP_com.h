
// maximum UDP buffer size
#define NMAX_UDP_BUFFER 30000

// maximum number of characters for addresses
#define NMAX_ADDRESS 128

int activate_network();

int deactivate_network();

int activate_socket(int port, char *local_IP_address, int &sock, int mode = 0);
// local_IP_address -- use NULL to accept messages for all interfaces
// mode = 0 -- transmit to only one computer (unicast)
// mode = 1 -- transmit to all computers on local network (broadcast)

int deactivate_socket(int &sock);

int recv(char *buffer, int &size, char *IP_address, int sock);

int send(char *buffer, int size, char *IP_address, int sock, int port);
