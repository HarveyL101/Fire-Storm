#ifndef FRAMER_HPP
#define FRAMER_HPP

#include <netinet/in.h>
#include <vector>


class PacketFramer
{
    private:
        const int MAX_PACKET           = 528; // Maximum observed size of network packets.
        const uint8_t CTFS_HEADER_SIZE = 16;  // ctfs marker + sequenceID + payload length + 8 unknown bytes.
        const uint8_t ETH_HEADER_SIZE  = 14;  // Observed ethernet header length.
        const uint8_t MIN_IP_HEADER    = 20;  // Observed IP header length.

    public:
        enum class Status { NeedMore, Complete, Invalid };

        struct Result {
            Status status;
            ssize_t length;
        };

        Result frame(const std::vector<uint8_t> &buffer);
        bool isCTFS(const std::vector<uint8_t> &header);
};

#endif