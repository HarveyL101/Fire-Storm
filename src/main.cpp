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
    uint8_t buffer[32 * 1024];    // 32kb buffer, for fewer recv() calls.
    ssize_t n;                    // Number of bytes held by buffer.
    
    // Creates, configures and binds a new socket. Listening to PORT 5733.
    if (NM.setupLS() != 0) { printf("Error calling NM.setuo().\n"); return -1; }
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
            
            if (r.status == PacketFramer::Status::Invalid) { /*Decide whether to forward or dump*/ break; }

            // Code for sending CTFS to state machine, otherwise forward the first -> r.length bytes ...
            if (r.status == PacketFramer::Status::Complete)
            {
                std::vector<uint8_t> packet(holding.begin(), holding.begin() + r.length);
                // CTFS packets are passed on for PDF writing.
                if (r.kind == PacketFramer::Kind::CTFS) { SM.pass(packet); }
                // Non-CTFS traffic is sent to the 5744 listening port.
                if (r.kind == PacketFramer::Kind::Forward) { NM.forward(packet); }
            }

            // Clear the now dispatched bytes and realign the start of the next with holding[0]
            holding.erase(holding.begin(), holding.begin() + r.length);
        }
    }

    // Tie off the program down here.

    // Process anything remaining in `holding`.

    // Check every forwarded frame has been send()-ed.

    // Close the 5744 sending port.

    // Write the PDF 

    // Close `client`.
    return 0;
}