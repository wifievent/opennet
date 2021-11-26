#ifndef UDPHDR_H
#define UDPHDR_H

#include "opennet.h"
#include "buf.h"
class UdpHdr
{
public:
    uint16_t sport;
    uint16_t dport;
    uint16_t len;
    uint16_t checksum;
    buf parseData(UdpHdr* udpHdr_);
};
typedef UdpHdr *PUdpHdr;
#endif // UDPHDR_H
