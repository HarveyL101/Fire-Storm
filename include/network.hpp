#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdint>

class NetworkManager
{
    private:
        sockaddr_in address{};         // Zero initialised.
        int server_socket = -1;        // Original listening socket.
        const uint16_t PORT_LS = 5733; // Listening Port.
        const uint16_t PORT_FD = 5744; // Forwarding Port.
        const uint8_t MAX_CON = 16;    // Maximum connections to be queued.

    public:
        int setup(void);
        int begin(void);
        void forward(void);
        void terminate(uint8_t PORT);
};

#endif