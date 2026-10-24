#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdint>
#include <vector>

class NetworkManager
{
    private:
        sockaddr_in ls_address{};      // Zero initialised.
        sockaddr_in fd_address{};      // Zero initialised. 
        int server_socket      = -1;   // Original listening socket.
        int forward_socket     = -1;   // Original Forwarding socket.
        const uint16_t PORT_LS = 5733; // Listening Port.
        const uint16_t PORT_FD = 5744; // Forwarding Port.
        const uint8_t MAX_CON  = 16;   // Maximum connections to be queued.

    public:
        int setupLS(void);
        int setupFD(void);
        int begin(void);
        int forward(const std::vector<uint8_t> &packet);
        // void terminate(uint8_t PORT);
};

#endif