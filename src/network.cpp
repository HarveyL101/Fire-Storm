#include "network.hpp"

int NetworkManager::setupLS(void)
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0); // Creation of socket File Descriptor.
    if (server_socket == -1) { printf("Failed to create a socket\n"); return -1; }

    const int yes = 1;

    // SOL_SOCKET to specify what level the configurations are interpreted at (Socket lvl). 
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        printf("Failed to set listening socket options\n");
        return -1;
    }

    ls_address.sin_family      = AF_INET;           // IPv4 address family.
    ls_address.sin_port        = htons(PORT_LS);    // Listening port (5733) in network byte order.
    ls_address.sin_addr.s_addr = htonl(INADDR_ANY); // Will bind to any available local IPv4 interface.

    if (bind(server_socket, reinterpret_cast<sockaddr*>(&ls_address), sizeof(ls_address)) == -1)
    {
        printf("Failed to bind to PORT(%d)\n", PORT_LS);
        return -1;
    };

    if (listen(server_socket, MAX_CON) == -1)
    {
        printf("Failed to listen @:\n\tPORT: %d\n\tSocket: %d\n", PORT_LS, server_socket);
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

int NetworkManager::setupFD(void)
{
    // Create a socket to transmit information from.
    forward_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (forward_socket == -1) { printf("Failed to create a forwarding socket.\n"); return -1; }
    
    // Connect to PORT 5744 (lazy connect).
    fd_address.sin_family      = AF_INET;           // IPv4 address family.
    fd_address.sin_port        = htons(PORT_FD);    // Forwarding destination (PORT 5744).
    fd_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Will bind to any availale local IPv4 interface.

    if (connect(forward_socket, reinterpret_cast<sockaddr*>(&fd_address), sizeof(fd_address)) == -1)
    {
        printf("Failed to connect to PORT(%d).\n", PORT_FD);
        return -1;
    }

    return 0;
}

int NetworkManager::forward(const uint8_t *data, size_t length)
{    
    if (forward_socket == -1)
    {
        // Only connecting once the port is required.
        if (setupFD() == -1) { return -1; }
    }
    // Send the packet to its destination.
    size_t sent = 0;
    while (sent < length)
    {
        ssize_t result = send(
            forward_socket,
            data + sent,
            length - sent,
            MSG_NOSIGNAL
        );
        // Negative return value indicates an error.
        if (result == -1)
        {
            printf("Failed to send message:\n\tSocket: %d\n\tPort: %d\n\tBytes: %zu\n", 
                    forward_socket, ntohs(fd_address.sin_port), length);
            return -1;
        }
        // 0 return value indicates that no transmission progress was made.
        if (result == 0)
        {
            printf("Connection closed during transmission.\n");
            return -1;
        }
        // Increments `sent` by the actual count of bytes successfully transmitted.
        sent += static_cast<size_t>(result);
    }

    return 0;
}

void NetworkManager::closeForward(void)
{
    if (forward_socket != -1)
    {
        close(forward_socket);
        forward_socket = -1;
    }
}