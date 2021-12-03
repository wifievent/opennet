#include "buf.h"
#pragma pack(push, 1)
struct UdpHdr
{
    uint16_t sport_;
    uint16_t dport_;
    uint16_t len_;
    uint16_t checksum_;
    static Buf parseData(UdpHdr* udpHdr_);

    uint16_t sport() { return sport_; }
    uint16_t dport() { return dport_; }
    uint16_t len() { return len_; }
    uint16_t checksum() { return checksum_; }
};
typedef UdpHdr *PUdpHdr;
#pragma pack(pop)
