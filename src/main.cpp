#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#include "network.hpp"
#include "framer.hpp"

int main(void) 
{
    NetworkManager NM;
    PacketFramer PF;

    if (NM.setup() != 0) { printf("Error calling NM.setuo().\n"); return -1; }

    int client = NM.begin();

    if (client == -1) { printf("Error calling NM.begin().\n"); return -1; }

    while ((PF.held = recv(client, PF.buffer, sizeof(PF.buffer), 0)) > 0)
    {
        printf("--- recv returned %zd bytes ---\n", PF.held);
        for (ssize_t i = 0; i < PF.held; ++i)
        {
            printf("%02x ", static_cast<unsigned char>(PF.buffer[i]));
            if ((i + 1) % 16 == 0) printf("\n");
        }
        
    }
    return 0;
}