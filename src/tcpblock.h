#include "arpspoof.h"
struct TcpBlock : StateObj
{
public:
    bool forwardRst_ = false;
    bool forwardFin_ = false;
    bool backwardRst_ = false;
    bool backwardFin_ = false;

    Intf intf_;
    bool enabled_ = true;
    PcapDevice* writer_;
    string backwardFinMsg_;
    int bufSize_ = 32768;
    void block(Packet* packet);

protected:
    EthPacket blockEthPacket_;
    IpPacket blockIpPacket_;

    uint8_t* blockBuf_{nullptr};
    bool doOpen() override;
    bool doClose() override;

    void sendForwardBlockPacket(Packet* packet);
    void sendBackwardBlockPacket(Packet* packet);
};
