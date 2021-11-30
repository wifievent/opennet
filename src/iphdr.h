#pragma once
#include "buf.h"

struct IpHdr
{
    enum {
      TCP = 0x06,
      UDP = 0x17,
      ICMP = 0x01
    };
    uint8_t hhlen_; //header length
    uint8_t version_;
    uint16_t tlen_; //tot length
    uint16_t identification_;
    uint8_t flag_;
    uint8_t offset_;
    uint8_t ttl_;
    uint8_t protocol_;
    uint16_t checksum_;
    Ip sip_;
    Ip dip_;

    uint8_t hlen(){return hhlen_;}
    uint8_t p(){return protocol_;}
    uint16_t tlen(){return ntohs(tlen_);}
    Ip dip(){ return dip_; }
    Ip sip(){ return sip_; }
    uint16_t sum(){ return ntohs(checksum_);}
    static uint16_t calcChecksum(IpHdr* iphdr);
};
typedef IpHdr *PIpHdr;

