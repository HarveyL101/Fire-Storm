#include <netinet/in.h>
#include <stdio.h>

#include <src/network.cpp>

class PacketFramer:: {
    private:
        uint8_t CTFS_LENGTH = 16; // Desired packet's header length.
    public:
        char* buffer[1024]; 
        ssize_t held; // Number of bytes held by buffer.

        new Network
};