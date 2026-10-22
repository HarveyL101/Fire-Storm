#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

// Listening Port.
// Forwarding Port.

class NetworkManager 
{
    private:
        sockaddr_in address{};   // Zero initialised.
        uint16_t PORT_LS = 5733; // Listening Port.
        uint16_t PORT_FD = 5744; // Forwarding Port.

    public:
        int32_t server_socket; // Original listening socket.

        int8_t setup(void) 
        {
            server_socket = socket(AF_INET, SOCK_STREAM, 0); // Creation of socket File Descriptor.
            if (server_socket == -1) { printf("Failed to create a socket\n"); return -1; }

            uint8_t yes = 1;

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

            return 0;
        }

        int32_t begin(void)
        {
            int32_t client = accept(server_socket, nullptr, nullptr);
            if (client == -1) 
            {
                printf("Failed to accept client at File Descriptor: %d", 
                        server_socket); 
                return -1;
            }
            return client;
        }

        void forward(void)
        {
            // This will forward the traffic passed to it.
        }
};