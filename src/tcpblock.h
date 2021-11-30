#include "intf.h"
#include "obj.h"
#include "pcapdevice.h"
#include "ethpacket.h"
struct TcpBlock : Obj
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

    gbyte* blockBuf_{nullptr};
    bool doOpen() override;
    bool doClose() override;

    void sendForwardBlockPacket(Packet* packet);
    void sendBackwardBlockPacket(Packet* packet);
};
