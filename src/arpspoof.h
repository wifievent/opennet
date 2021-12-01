#include "spdlog/spdlog.h"
#include "flow.h"
#include "intf.h"
#include "pcapdevice.h"
struct EthArpPacket
{
    EthHdr ethHdr_;
    ArpHdr arpHdr_;
};

struct Arpspoof : PcapDevice
{
    PcapDevice device_;
    Intf* intf_;
    Mac gatewayMac_;

    Arpspoof(){};

    void sendInfect(Flow flow);
    void sendRecover(Flow flow);
    void sendRelay(Flow flow);
    Packet::Result relay(Packet* packet);
};

