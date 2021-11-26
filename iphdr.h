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
    uint8_t version_;
    uint8_t len_;
    uint16_t hlen_;
    uint16_t identification_;
    uint8_t flag_;
    uint8_t offset_;
    uint8_t ttl_;
    uint8_t protocol_;
    uint16_t checksum_;
    Ip sip_;
    Ip dip_;

    uint8_t len(){return len_;}
    uint8_t p(){return protocol_;}
    uint16_t hl(){return ntohs(hlen_);}
    Ip dip(){ return dip_; }
    Ip sip(){ return sip_; }
    uint16_t sum(){ return ntohs(checksum_);}
    uint16_t calcChecksum(IpHdr* iphdr);
};
typedef IpHdr *PIpHdr;

#endif // IPHDR_H
