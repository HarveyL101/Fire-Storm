#ifndef FRAMER_HPP
#define FRAMER_HPP

#include <netinet/in.h>
#include <vector>
#include <cstring>


class PacketFramer
{
    private:

    public:
        static constexpr size_t MAX_PACKET       = 528; // Maximum observed size of network packets.
        static constexpr size_t CTFS_HEADER_SIZE = 16;  // ctfs marker(4) + sequenceID(2) + payload length(2) + unknown bytes(6) + byte buffer (2).
        static constexpr size_t ETH_HEADER_SIZE  = 14;  // Observed ethernet header length.
        static constexpr size_t MIN_IP_HEADER    = 20;  // Observed IP header length.
        
        enum class Status { NeedMore, Complete, Invalid };
        enum class Kind   { None, CTFS, Forward };

        struct Result 
        {
            Status status  = Status::NeedMore;
            Kind   kind    = Kind::None;
            size_t length  = 0; // Whole packet, only valid on a complete transmission.
            uint16_t seqID = 0; // 0 is only valid for complete CTFS packets.
        };

        Result frame(const std::vector<uint8_t> &buffer);
        bool isCTFS(const std::vector<uint8_t> &header);
};

#endif