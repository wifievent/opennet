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
    string forwardFinMsg_;
    string backwardFinMsg_;
    int bufSize_ = 32768;
    void block(Packet* packet);
};

