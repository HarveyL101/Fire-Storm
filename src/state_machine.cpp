#include "state_machine.hpp"

void StateMachine::pass(std::span<const uint8_t> payload, uint16_t seqID)
{
    // Check this is not the termination packet.
    if (payload.empty()) { return; /* Decide on int or ENUM return flag. */ }

    
}

void StateMachine::map(std::span<const uint8_t> payload, uint16_t seqID)
{
    
}