#pragma once

#include <cstdint>
#include <vector>

// Helper function for retrieving two adjacent bytes from `buffer` at location `offset`. Returned as an integer.
inline uint16_t byte_slice(const std::vector<uint8_t> &buffer, size_t offset)
{
    // Shifting left by 8 bits creates the space for `buffer[offset+1]`
    // to be appended to the second byte.
    return static_cast<uint16_t>((buffer[offset] << 8) | buffer[offset + 1]);
}