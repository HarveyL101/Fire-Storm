#ifndef FRAMER_HPP
#define FRAMER_HPP

#include <netinet/in.h>

class PacketFramer
{
    private:
        const uint8_t CTFS_LEN = 16; // Desired packets' header length. 
        static const int BUFFER_CAP = 1024;
    public:
        unsigned char buffer[BUFFER_CAP]; // Char because the byte stream is received individually.
        ssize_t held = 0; // Number of bytes held by buffer.

        void frame(void);
};

#endif