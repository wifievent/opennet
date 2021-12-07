#pragma once
#include "pcapdevice.h"
#include "flow.h"
#include "dhcphdr.h"
struct EthArpPacket
{
    EthHdr ethHdr_;
    ArpHdr arpHdr_;
};

struct ArpSpoof : PcapDevice
{
    std::condition_variable Cv_;
    Ip myIp_;
    Mac myMac_;
    int sendSleepTime_{3}; // 3 msecs
    int rescanSleepTime_{600000}; // 10 minutes
    Ip gwIp_;
    Mac gwMac_;
    std::mutex m;
    EthArpPacket packet;
    ArpSpoof(){};

    bool prepare();
    bool sendInfect(Flow flow);
    bool sendRecover(Flow flow);
    Packet::Result relay(Packet* packet) override;
    bool sendQuery(Ip tip);
    void hostScan();
    Flow detect(Packet* packet);
    bool processArp(EthHdr* ethHdr, ArpHdr* arpHdr, Mac* mac, Ip* ip);
    bool processDhcp(Packet* packet, Mac* mac, Ip* ip);
    void makeArppacket(Mac dmac, Mac smac, Mac tmac, Ip tip, Ip sip);
protected:
    bool doOpen() override;
    bool doClose() override;
};
