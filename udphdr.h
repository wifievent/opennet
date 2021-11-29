#include "opennet.h"
#include "buf.h"
struct UdpHdr final
{
    uint16_t sport;
    uint16_t dport;
    uint16_t len;
    uint16_t checksum;
    static Buf parseData(UdpHdr* udpHdr_);
};
typedef UdpHdr *PUdpHdr;

