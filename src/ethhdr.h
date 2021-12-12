#pragma once
#include "buf.h"
#pragma pack(push, 1)
struct EthHdr
{
    enum {
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
#pragma pack(pop)
