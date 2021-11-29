#include "packet.h"
#include "opennet.h"
#include "intf.h"
#include "obj.h"
#include "pcapdevice.h"
struct TcpBlock : Obj
{
public:
    Intf intf_;
    bool enabled_ = true;
    bool forwardRst_ = true;
    bool backwardFin_ = false;
    PcapDevice* writer_;
    string backwardFinMsgStr_;
    int bufSize_ = 32768;

protected:
    gbyte* blockBuf_{nullptr};
    bool doOpen() override;
    bool doClose() override;

    enum Direction {
        Forward,
        Backward
    };

    enum BlockType {
        Rst,
        Fin
    };
    void block(Packet* packet);
    void sendBlockPacket(Packet* packet, Direction direction, BlockType blockType, uint32_t seq, uint32_t ack, string msg = "");
};

