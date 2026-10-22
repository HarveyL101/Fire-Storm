#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdint>

class NetworkManager
{
    private:
        sockaddr_in address{};
        uint16_t PORT_LS = 5733;
        uint16_t PORT_FD = 5744;

    public:
        uint32_t server_socket;

        int8_t setup(void);
        int32_t begin(void);
};

#endif