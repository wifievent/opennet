#include "packet.h"
#include "opennet.h"
#include "intf.h"
#include "obj.h"
#include "pcapdevice.h"
#include "ethpacket.h"
#include "ippacket.h"
struct TcpBlock : Obj
{
public:
    enum BlockType {
        None,
        Rst,
        Fin
    };

    Intf intf_;
    bool enabled_ = true;
    PcapDevice* writer_;
    string backwardFinMsgStr_;
    int bufSize_ = 32768;
    BlockType forwardBlockType_{Rst};
    BlockType backwardBlockType_{Rst};

protected:
    EthPacket blockEthPacket_;
    IpPacket blockIpPacket_;

    gbyte* blockBuf_{nullptr};
    bool doOpen() override;
    bool doClose() override;

    enum Direction {
        Forward,
        Backward
    };

    void block(Packet* packet);
    void sendBlockPacket(Packet* packet, Direction direction, BlockType blockType, uint32_t seq, uint32_t ack, string msg = "");
};
