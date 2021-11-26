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
    Mac dmac_;
    Mac smac_;
    uint16_t type_;

    uint16_t type(){ return ntohs(type_); }
    Mac smac(){ return smac_; }
    Mac dmac(){ return dmac_; }
};
typedef EthHdr *PEthHdr;
#endif // ETHHDR_H
