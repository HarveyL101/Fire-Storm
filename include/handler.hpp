#ifndef handler_HPP
#define handler_HPP

#include <stdint.h>
#include <vector>
#include <unordered_map>

class Handler
{
    private:
        std::unordered_map<uint16_t, std::vector<uint8_t>> chunks; // Stores the complete PDF.
    public:
        int map(std::vector<uint8_t> payload, uint16_t seqID);
        int build();
};

#endif