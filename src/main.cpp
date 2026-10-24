#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#include "network.hpp"
#include "framer.hpp"
#include "state_machine.hpp"

int main(void) 
{
    NetworkManager NM;
    PacketFramer PF;
    StateMachine SM;

    std::vector<uint8_t> holding; // Iterable memory block for splicing packets from.
    uint8_t buffer[32 * 1024]; // 32kb buffer, for fewer recv() calls.
    ssize_t n; // Number of bytes held by buffer.
    
    // Creates, configures and binds a new socket. Listening to PORT 5733.
    if (NM.setup() != 0) { printf("Error calling NM.setuo().\n"); return -1; }
    // Accepts any new connections, creating a new socket for data to stream to
    // while maintaining the original listener.
    int client = NM.begin();

    if (client == -1) { printf("Error calling NM.begin().\n"); return -1; }

    while ((n = recv(client, buffer, sizeof(buffer), 0)) > 0)
    {
        // Append new bytes to the end of the holding block.
        holding.insert(holding.end(), buffer, buffer + n);
        // Cut off every complete packet.
        for (;;)
        {
            PacketFramer::Result r = PF.frame(holding);
            if (r.status == PacketFramer::Status::NeedMore) break;
            
            // Code for sending CTFS to state machine, otherwise forward the first -> r.length bytes ...
            if (r.status == PacketFramer::Status::Complete)
            {
                // Check the length of r.length for the protocol used.
                // | ETH == number | IP == number |

                // Pass the completed packet to the state machine.
                std::vector<uint8_t> packet(holding.begin(), holding.begin() + r.length);   
                SM.pass(packet);

                break;
            }

            // Clear the now dispatched bytes and realign the start of the next with holding[0]
            holding.erase(holding.begin(), holding.begin() + r.length);
        }
    }
    return 0;
}