#ifndef TCPHDR_H
#define TCPHDR_H
#include "opennet.h"
#include "buf.h"
#include "iphdr.h"

class TcpHdr
{
public:
    uint16_t sport_;
    uint16_t dport_;
    uint8_t tlen;
    uint32_t seqnum;
    uint32_t acknum;
    uint8_t hlen;
    uint16_t flag;
    uint16_t checksum;
    uint16_t upointer;

    uint16_t dport(){ return ntohs(dport_); }
    uint16_t sport(){ return ntohs(sport_); }
    uint8_t off(){ return tlen; }
    buf parseData(PIpHdr ipHdr_,TcpHdr* tcpHdr_);
};
typedef TcpHdr *PTcpHdr;

#endif // TCPHDR_H
