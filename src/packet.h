#pragma once
#include "ethhdr.h"
#include "tcphdr.h"
#include "arphdr.h"
#include "udphdr.h"
#include "icmphdr.h"
struct Packet
{
    Packet() { clear();}
    enum Result{
      Ok = 1,
      Fail = -1,
      Eof = -2,
      None = 0,
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

    struct timeval ts_;
    Buf buf_;
    Buf udpData_;
    Buf tcpData_;

    struct {
        bool block_{false};
    } ctrl;

    Dlt dlt() { return dlt_; };
    static Dlt intToDlt(int dataLink);
    void clear();
    virtual void parse();
    void copyFrom(Packet* source, Buf newBuf);
protected:
    Dlt dlt_{Null};
};

