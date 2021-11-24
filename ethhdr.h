#ifndef ETHHDR_H
#define ETHHDR_H
#include "opennet.h"
#include "ip.h"
#include "mac.h"
class ethhdr
{
public:
    enum{
        Arp = 0x806,
        Ip4 = 0x800,
    };
    mac dmac_;
    mac smac_;
    uint16_t type_;
};

#endif // ETHHDR_H
