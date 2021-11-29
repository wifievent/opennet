#include "opennet.h"
#include "ethhdr.h"
#include "iphdr.h"
#include "tcphdr.h"
#include "arphdr.h"
#include "udphdr.h"
#include "icmphdr.h"
#include "buf.h"
struct Packet
{
    Packet() { clear();}
    enum Result{
      Ok = 1,
      Fail = -1
    };

    typedef enum {
        Eth, // DLT_EN10MB (1)
        Ip, // DLT_RAW (228)
        Dot11, // DLT_IEEE802_11_RADIO (127)
        Null, // DLT_NULL (0)
    } Dlt; // data link type

    EthHdr* ethHdr_{nullptr};
    IpHdr* ipHdr_{nullptr};
    UdpHdr* udpHdr_{nullptr};
    TcpHdr* tcpHdr_{nullptr};
    ArpHdr* arpHdr_{nullptr};
    IcmpHdr* icmpHdr_{nullptr};

    Buf buf_;
    Buf udpData_;
    Buf tcpData_;

    struct {
        bool block_{false};
    } ctrl;
    static Dlt intToDlt(int dataLink);
    void clear();
    virtual void parse();

protected:
	Dlt dlt_{Null};
};
