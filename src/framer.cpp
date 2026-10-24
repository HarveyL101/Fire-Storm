#include <netinet/in.h>
#include <vector>
#include <cstring>

#include "framer.hpp"

bool PacketFramer::isCTFS(const std::vector<uint8_t> &buffer)
{
    // Checks that the vector is appropriately sized and starts with the sequence "CTFS".
    return buffer.size() >= 4 && std::memcmp(buffer.data(), "CTFS", 4) == 0;
}

PacketFramer::Result PacketFramer::frame(const std::vector<uint8_t> &buffer)
{
    // Too short to recognise the packet type. Every real packet would be >= 16 bytes.
    if (buffer.size() < 4) return {Status::NeedMore, 0};

    // Check if the first 4 bytes are "CTFS"
    if (isCTFS(buffer))
    {
        if (buffer.size() < CTFS_HEADER_SIZE) return {Status::NeedMore, 0}; 

        uint16_t length;
        std::memcpy(&length, buffer.data() + 6, sizeof(length));
        // Packet length (ETH), CTFS header + payload.
        size_t total = CTFS_HEADER_SIZE + ntohs(length); 
        
        // Confirming that the packet is not split between recv() calls.
        if (buffer.size() < total) return {Status::NeedMore, 0}; 

        return {Status::Complete, total};
    }

    // Anything else is expected to be Ethernet -> IPv4 -> payload (Observed).
    if (buffer.size() < 18) return {Status::NeedMore, 0};

    // Check if eth.type is not IPv4
    if (buffer[12] != 0x08 || buffer[13] != 0x00) return {Status::Invalid, 0};
    // Check if the IP version is not 4
    if ((buffer[14] >> 4) != 4)                   return {Status::Invalid, 0};

    uint16_t ipLength;
    std::memcpy(&ipLength, buffer.data() + 16, sizeof(ipLength));
    // 14-byte Ethernet header + IP total length
    size_t total = ETH_HEADER_SIZE + ntohs(ipLength);

    // 
    if (total < ETH_HEADER_SIZE + MIN_IP_HEADER || total > MAX_PACKET) return {Status::Invalid, 0};
    // Confirming that the packet is not split between recv() calls.
    if (buffer.size() < total) return {Status::NeedMore, 0};

    return {Status::Complete, total};
}