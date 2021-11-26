
#ifndef PACKET_H
#define PACKET_H
#include "opennet.h"
#include "ethhdr.h"
#include "iphdr.h"
#include "tcphdr.h"
#include "arphdr.h"
#include "udphdr.h"
#include "buf.h"
class packet
{
public:
    enum Result{
      Ok,
      Fail
    };
    EthHdr* ethHdr_{nullptr};
    IpHdr* ipHdr_{nullptr};
    UdpHdr* udpHdr_{nullptr};
    TcpHdr* tcpHdr_{nullptr};
    ArpHdr* arpHdr_{nullptr};
    buf udpData;
    buf tcpData;

    struct {
        bool block_{false};
    } ctrl;
};

#endif // PACKET_H

