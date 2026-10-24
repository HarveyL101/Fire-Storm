#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <stdint.h>
#include <vector>

class StateMachine
{
    private:
    public:
        void pass(std::vector<uint8_t> packet);
};

#endif