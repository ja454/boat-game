
int activate_socket6(int port, char *local_IP_address, int &sock);
// local_IP_address -- use NULL to accept messages for all interfaces

int deactivate_socket6(int &sock);

int recv6(char *buffer, int &size, char *IP_address, int sock);

int send6(char *buffer, int size, char *IP_address, int sock, int port);
