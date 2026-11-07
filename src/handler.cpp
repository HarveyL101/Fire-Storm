#include <fstream>

#include "handler.hpp"

// Stores the received payloads 
int Handler::map(std::vector<uint8_t> payload, uint16_t seqID)
{
    if (chunks.count(seqID) > 0) 
    {
        printf("Duplicate sequence ID found at chunks[%d].\n", seqID);
        return -1; // Identical seqID's should not be being sent.
    }

    chunks.insert({seqID, payload});

    return 0;
}

int Handler::build()
{
    if (chunks.empty()) return -1;

    // the largest key will always contain the terminator (empty payload)
    int totalChunks = chunks.size(); 

    std::ofstream output("build/transmission.pdf", std::ios::binary);

    for (int i = 0; i < totalChunks - 1; i++)
    {
        const auto &chunk = chunks.at(i);
        
        output.write(reinterpret_cast<const char*>(chunk.data()), static_cast<std::streamsize>(chunk.size()));
    }

    return 0;
}