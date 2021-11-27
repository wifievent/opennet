#include "opennet.h"
#include "ethhdr.h"
#include "iphdr.h"
#include "tcphdr.h"
#include "arphdr.h"
#include "udphdr.h"
#include "buf.h"
struct Packet
{
    enum Result{
      Ok,
      Fail
    };
    EthHdr* ethHdr_{nullptr};
    IpHdr* ipHdr_{nullptr};
    UdpHdr* udpHdr_{nullptr};
    TcpHdr* tcpHdr_{nullptr};
    ArpHdr* arpHdr_{nullptr};

    Buf udpData;
    Buf tcpData;

    struct {
        bool block_{false};
    } ctrl;
};
