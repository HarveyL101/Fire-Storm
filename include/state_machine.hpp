#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <stdint.h>
#include <vector>
#include <span>
#include <map>

class StateMachine
{
    private:
        std::map<uint16_t, std::span<const uint8_t>> pdf; // Stores the complete PDF.
        size_t offset = 0; // Tracks the position of the "end" of the current PDF.
    public:
        void pass(std::span<const uint8_t> payload, uint16_t seqID);
        void map(std::span<const uint8_t> payload, uint16_t seqID);
};

#endif