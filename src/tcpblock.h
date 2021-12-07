#pragma once
#include "pcapdevice.h"
struct TcpBlock : StateObj
{
public:
    bool forwardRst_ = false;
    bool forwardFin_ = false;
    bool backwardRst_ = false;
    bool backwardFin_ = false;
    int sendSleepTime_{50}; // 50 msecs

    Intf intf_;
    bool enabled_ = true;
    PcapDevice* writer_;
    string backwardFinMsg_;
    int bufSize_ = 32768;
    void block(Packet* packet);

protected:
    EthPacket blockEthPacket_;
    IpPacket blockIpPacket_;

    bool doOpen() override;
    bool doClose() override;

    bool sendForwardBlockPacket(Packet* packet);
    bool sendBackwardBlockPacket(Packet* packet);
};
