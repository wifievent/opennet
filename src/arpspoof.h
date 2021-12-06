#pragma once
#include "pcapdevice.h"
#include "flow.h"
#include "dhcphdr.h"
struct ArpSpoof : PcapDevice
{
    std::condition_variable Cv_;
    Ip myIp_;
    Mac myMac_;
    PcapDevice* device_;
    Mac gatewayMac_;
    int sendSleepTime_{50}; // 50 msecs
    int rescanSleepTime_{600000}; // 10 minutes

    ArpSpoof(){};

    bool sendInfect(Flow flow);
    void sendRecover(Flow flow);
    Packet::Result relay(Packet* packet) override;
    bool sendQuery(Ip tip);
    void hostScan();
    Flow detect(Packet* packet);
    bool processArp(EthHdr* ethHdr, ArpHdr* arpHdr, Mac* mac, Ip* ip);
    bool processDhcp(Packet* packet, Mac* mac, Ip* ip);

protected:
    Ip gwIp_;
    Mac gwMac_;
    bool doOpen() override;
    bool doClose() override;
};

struct EthArpPacket
{
    struct TimeSet : std::set<Flow> { //for check time
        std::mutex m_;
    } timeSet_;

    std::string redirectpage_ = "http://wifievent.io";
    ArpSpoof arpspoof_;
    EthHdr ethHdr_;
    ArpHdr arpHdr_;
};
