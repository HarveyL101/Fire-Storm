#include "framer.hpp"
#include "util/snippet16.hpp"

namespace
{
    PacketFramer::Result needMore() { return{}; }
    PacketFramer::Result invalid() { return{PacketFramer::Status::Invalid, PacketFramer::Kind::None, 0, 0}; }
    PacketFramer::Result complete(PacketFramer::Kind k, size_t length, uint16_t seqID = 0)
    {
        return {PacketFramer::Status::Complete, k, length, seqID};
    }
}

bool PacketFramer::isCTFS(const std::vector<uint8_t> &buffer)
{
    // Checks that the vector is appropriately sized and starts with the sequence "CTFS".
    return buffer.size() >= 4 && std::memcmp(buffer.data(), "CTFS", 4) == 0;
}

PacketFramer::Result PacketFramer::frame(const std::vector<uint8_t> &buffer)
{
    // Too short to recognise the packet type. Every real packet would be >= 16 bytes.
    if (buffer.size() < 4) return needMore();

    // Check if the first 4 bytes are "CTFS"
    if (isCTFS(buffer))
    {
        if (buffer.size() < CTFS_HEADER_SIZE) return needMore(); 
        // Checking 
        const uint16_t seqID = snippet16(buffer, 4);
        const size_t   total = CTFS_HEADER_SIZE + snippet16(buffer, 6);
        // Upper bounds checking for packet length.
        if (total > MAX_PACKET) return invalid();
        // Confirming that the packet is not split between recv() calls.
        if (buffer.size() < total) return needMore(); 

        return complete(Kind::CTFS, total, seqID);
    }

    // Anything else is expected to be Ethernet -> IPv4 -> payload (Observed).
    if (buffer.size() < 18) return needMore();

    // Check if eth.type is not IPv4
    if (buffer[12] != 0x08 || buffer[13] != 0x00) return invalid();
    // Check if the IP version is not 4
    if ((buffer[14] >> 4) != 4)                   return invalid();

    uint16_t ipLength;
    std::memcpy(&ipLength, buffer.data() + 16, sizeof(ipLength));
    // 14-byte Ethernet header + IP total length
    size_t total = ETH_HEADER_SIZE + ntohs(ipLength);

    // Lower and upper bounds checking for header lengths.
    if (total < ETH_HEADER_SIZE + MIN_IP_HEADER || total > MAX_PACKET) return invalid();
    // Confirming that the packet is not split between recv() calls.
    if (buffer.size() < total) return needMore();

    return complete(Kind::Forward, total);
}