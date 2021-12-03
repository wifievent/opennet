#include "iphdr.h"
#pragma pack(push, 1)
struct TcpHdr
{
public:
    uint16_t sport_;
    uint16_t dport_;
    uint32_t seqnum_;
    uint32_t acknum_;
    uint8_t hlen_;
    uint16_t flag_;
    uint16_t win_;
    uint16_t checksum_;
    uint16_t upointer_;

    uint16_t dport(){ return ntohs(dport_); }
    uint16_t sport(){ return ntohs(sport_); }
    uint8_t off(){ return (hlen_ & 0xF0) >> 4; }
    uint16_t win(){ return ntohs(win_); }
    uint16_t sum(){ return ntohs(checksum_); }
    uint16_t flag(){ return ntohs(flag_); }
    uint32_t seqnum(){ return ntohl(seqnum_); }
    uint32_t acknum(){ return ntohl(acknum_); }

    static Buf parseData(PIpHdr ipHdr_,TcpHdr* tcpHdr_);
    static uint16_t calcChecksum(IpHdr* iphdr, TcpHdr* tcphdr);
    // Flag(flag_)
    enum: uint8_t {
        Urg = 0x20,
        Ack = 0x10,
        Psh = 0x08,
        Rst = 0x04,
        Syn = 0x02,
        Fin = 0x01
    };
};
typedef TcpHdr *PTcpHdr;
#pragma pack(pop)
