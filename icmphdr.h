#ifndef ICMPHDR_H
#define ICMPHDR_H

#include "opennet.h"
class IcmpHdr
{
public:
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint8_t unused[4];
};
typedef IcmpHdr* PIcmpHdr;
#endif // ICMPHDR_H
