#pragma once
#include "pcapdevice.h"
struct TcpBlock : StateObj
{
public:
    enum BlockType {
        None,
        Rst,
        Fin
    };

    PcapDevice* writer_{nullptr};

    bool enabled_{true};
    BlockType forwardBlockType_{Rst};
    BlockType backwardBlockType_{Rst};
    string forwardFinMsg_;
    string backwardFinMsg_;
    int bufSize_{32768};

    void block(Packet* packet);
    ~TcpBlock() override { }

protected:
    bool doOpen() override;
    bool doClose() override;

protected:

    EthPacket blockEthPacket_;
    IpPacket blockIpPacket_;
    unsigned char* blockBuf_{nullptr};

    enum Direction {
        Forward,
        Backward
    };

    void sendBlockPacket(Packet* packet, Direction direction, BlockType blockType, uint32_t seq, uint32_t ack, std::string msg = "");
    Mac myMac_;
};
