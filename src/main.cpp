#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <span>

#include "network.hpp"
#include "framer.hpp"
#include "handler.hpp"

int main(void) 
{
    NetworkManager NM;
    PacketFramer PF;
    Handler SM;

    std::vector<uint8_t> holding; // Iterable memory block for splicing packets from.
    uint8_t buffer[32 * 1024];    // 32kb buffer, for fewer recv() calls.
    ssize_t n;                    // Number of bytes held by buffer.
    
    // Creates, configures and binds a new socket. Listening to PORT 5733.
    if (NM.setupLS() != 0) { printf("Error calling NM.setup().\n"); return -1; }
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
            PacketFramer::Result res = PF.frame(holding);

            if (res.status == PacketFramer::Status::NeedMore) break;

            if (res.status == PacketFramer::Status::Invalid)
            {
                printf("A packet's header Length/ Type is invalid, exiting the program now...\nClosing connections now...\n");
                // Terminate here.
                
                break;
            }

            if (res.status == PacketFramer::Status::Complete)
            {
                // CTFS packets are passed on for PDF writing.
                if (res.kind == PacketFramer::Kind::CTFS)
                {
                    // Copies framed CTFS packet (Excl. header).
                    std::vector<uint8_t> payload(holding.data() + PacketFramer::CTFS_HEADER_SIZE, holding.data() + res.length);
                    if (SM.map(payload, res.seqID) != 0) return -1; 
                } else {
                    // Non-CTFS traffic is sent to the 5744 listening port.
                    if (NM.forward(holding.data(), res.length) != 0) return -1;
                }
            }

            // Clear the now dispatched bytes and realign the start of the next with holding[0]
            holding.erase(holding.begin(), holding.begin() + res.length);
        }
    }

    // Tie off the program down here.
    if (n < 0) { printf("Error while closing the recv() stream: %d\n", errno); }
    // Log anything remaining in `holding`.
    printf("%zu bytes remaining in holding buffer.\n", holding.size());
    
    // Ensures the 5744 receiver sees the end of the byte stream.
    NM.closeForward();

    // Write the PDF 
    SM.build();

    close(client);

    return 0;
}