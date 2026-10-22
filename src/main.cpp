#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(void) 
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in address{}; 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(5733);

    bind(fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
    listen(fd, 16);

    int client = accept(fd, nullptr, nullptr);
    unsigned char buffer[256];
    ssize_t n;

    while ((n = recv(client, buffer, sizeof(buffer), 0)) > 0)
    {
        printf("--- recv returned %zd bytes ---\n", n);
        for (ssize_t i = 0; i < n; ++i)
        {
            printf("%02x ", buffer[i]);
            if ((i + 1) % 16 == 0) printf("\n");
        }
        
    }
    return 0;
}