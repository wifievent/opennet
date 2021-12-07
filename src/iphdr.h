#pragma once
#include "buf.h"
#pragma pack(push, 1)
struct IpHdr
{
    enum {
      TCP = 0x06,
      UDP = 0x17,
      ICMP = 0x01
    };
    uint8_t v_hl_;
    uint8_t tos_;
    uint16_t len_;
    uint16_t id_;
    uint16_t off_;
    uint8_t ttl_;
    uint8_t p_;
    uint16_t sum_;
    Ip sip_;
    Ip dip_;

    uint8_t v() { return (v_hl_ & 0xF0) >> 4; }
    uint8_t hl() { return v_hl_ & 0x0F; }
    uint8_t tos() { return tos_; }
    uint16_t len() { return ntohs(len_); }
    uint16_t id() { return ntohs(id_); }
    uint16_t off() { return ntohs(off_); }
    uint8_t ttl() { return ttl_; }
    uint8_t p() { return p_; }
    uint16_t sum() { return ntohs(sum_); }
    Ip sip() { return ntohl(sip_); }
    Ip dip() { return ntohl(dip_); }
    static uint16_t calcChecksum(IpHdr* iphdr);
};
typedef IpHdr *PIpHdr;
#pragma pack(pop)
