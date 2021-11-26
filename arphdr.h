#ifndef ARPHDR_H
#define ARPHDR_H
#include "opennet.h"
#include "mac.h"
#include "ip.h"
class ArpHdr
{
public:
    enum{
      ETHER = 0x0001,
      Request = 0x0001,
      Reply = 0x0002
    };
    uint8_t hrd_;
    uint16_t pro_;
    uint8_t hln_;
    uint8_t pln_;
    uint8_t op_;
    mac smac_;
    mac tmac_;
    Ip sip;
    Ip tip;
};
typedef ArpHdr *PArpHdr;
#endif // ARPHDR_H