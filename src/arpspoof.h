#include "spdlog/spdlog.h"
#include "flow.h"
#include "netinfo.h"
#include "dhcphdr.h"

struct EthArpPacket
{
    EthHdr ethHdr_;
    ArpHdr arpHdr_;
};

struct ArpSpoof : PcapDevice
{
    struct FlowList : std::list<Flow> { //for infection
        std::mutex m_;
    } flowList_;

    Ip myIp_;
    Mac myMac_;
    PcapDevice* device_;
    Intf* intf_;
    Mac gatewayMac_;
    int sendSleepTime_{50}; // 50 msecs
    int rescanSleepTime_{600000}; // 10 minutes
    unsigned long sendInterval_{1};

    ArpSpoof(){};

    bool sendInfect(Flow flow);
    void sendRecover(Flow flow);
    Packet::Result relay(Packet* packet) override;
    void removeFlows(Flow sender);
    bool sendQuery(Ip tip);
    void hostScan();
    bool sendArpInfectAll();
    void detect(Packet* packet);
    bool processDhcp(Packet* packet, Mac* mac, Ip* ip);

protected:
    Ip gwIp_;
    Mac gwMac_;
    bool doOpen() override;
    bool doClose() override;
};

