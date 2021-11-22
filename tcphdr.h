#ifndef TCPHDR_H
#define TCPHDR_H
#include "opennet.h"

class tcphdr
{
public:
    uint16_t sport;
    uint16_t dport;
    uint8_t tlen;
    uint32_t seqnum;
    uint32_t acknum;
    uint8_t hlen;
    uint16_t flag;
    uint16_t checksum;
    uint16_t upointer;
};

#endif // TCPHDR_H
