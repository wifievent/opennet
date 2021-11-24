#ifndef IPHDR_H
#define IPHDR_H
#include "opennet.h"
#include "ip.h"
#include "mac.h"
class Iphdr
{
public:
    enum{
      TCP = 0x06,
      UDP = 0x17
    };
    uint8_t version;
    uint8_t length;
    uint16_t tlen;
    uint16_t identification;
    uint8_t flag;
    uint8_t offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    ip sip;
    ip dip;
    uint8_t p(){return protocol;}
};

#endif // IPHDR_H
