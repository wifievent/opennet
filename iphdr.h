#ifndef IPHDR_H
#define IPHDR_H
#include "opennet.h"
#include "ip.h"
#include "mac.h"
class IpHdr
{
public:
    enum{
      TCP = 0x06,
      UDP = 0x17
    };
    uint8_t version;
    uint8_t len_;
    uint16_t hlen;
    uint16_t identification;
    uint8_t flag;
    uint8_t offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    Ip sip_;
    Ip dip_;

    uint8_t len(){return len_;}
    uint8_t p(){return protocol;}
    uint16_t hl(){return ntohs(hlen);}
    Ip dip(){ return dip_; }
    Ip sip(){ return sip_; }
};
typedef IpHdr *PIpHdr;

#endif // IPHDR_H
