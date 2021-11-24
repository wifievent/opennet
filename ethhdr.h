#ifndef ETHHDR_H
#define ETHHDR_H
#include "opennet.h"
#include "ip.h"
#include "mac.h"
class EthHdr
{
public:
    enum{
        Arp = 0x806,
        Ip4 = 0x800,
        Ip6 = 0x86DD
    };
    mac dmac_;
    mac smac_;
    uint16_t type_;
    uint16_t type(){ return ntohs(type_); }
    mac smac(){ return smac_; }
    mac dmac(){ return dmac_; }
};
typedef EthHdr *PEthHdr;
#endif // ETHHDR_H
