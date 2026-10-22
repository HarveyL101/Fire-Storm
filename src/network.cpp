#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#include "network.hpp"

// Listening Port.
// Forwarding Port.

int NetworkManager::setup(void)
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // Creation of socket File Descriptor.
    if (server_socket == -1) { printf("Failed to create a socket\n"); return -1; }

    const int yes = 1;

    // SOL_SOCKET to specify what level the configurations are interpreted at (Socket lvl). 
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        printf("Failed to set socket options\n");
        return -1;
    }

    address.sin_family = AF_INET;                // IPv4 address family.
    address.sin_port = htons(PORT_LS);           // Listening port in network byte order.
    address.sin_addr.s_addr = htonl(INADDR_ANY); // Will bind to any available local IPv4 interface.

    if (bind(server_socket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
    {
        printf("Failed to bind to PORT(%d)\n", PORT_LS);
        return -1;
    };

    if (listen(server_socket, MAX_CON) == -1)
    {
        printf("Failed to listen @:\n\tPORT: %d\n\tSocket: %d", PORT_LS, server_socket);
        return -1;
    }

    return 0;
}

int NetworkManager::begin(void)
{
    int client = accept(server_socket, nullptr, nullptr);
    if (client == -1) 
    {
        printf("Failed to accept client at File Descriptor: %d\n", server_socket); 
        return -1;
    }
    return client;
}

void NetworkManager::forward(void)
{
    // This will forward the traffic passed to it.
    // Connect to PORT 5744 here, to avoid a broken donnection during test.py's startup.
}

void NetworkManager::terminate(uint8_t PORT)
{
    // This will close off all active connections.
    PORT++; // meaningless change to get it to compile for a test.
    return;
}