#include "opennet.h"
#include "buf.h"
#include "iphdr.h"

struct TcpHdr
{
public:
    uint16_t sport_;
    uint16_t dport_;
    uint32_t seqnum_;
    uint32_t acknum_;
    uint8_t hlen_;
    uint16_t flag_;
    uint16_t checksum_;
    uint16_t upointer_;

    uint16_t dport(){ return ntohs(dport_); }
    uint16_t sport(){ return ntohs(sport_); }
    uint8_t off(){ return (hlen_ & 0xF0) >> 4; }
    uint16_t sum(){ return ntohs(checksum_); }
    uint32_t seqnum(){ return ntohl(seqnum_); }
    uint32_t acknum(){ return ntohl(acknum_); }
    static Buf parseData(PIpHdr ipHdr_,TcpHdr* tcpHdr_);
    static uint16_t calcChecksum(IpHdr* iphdr, TcpHdr* tcphdr);
};
typedef TcpHdr *PTcpHdr;
