#include "packet.h"
#include "opennet.h"
#include "intf.h"
struct TcpBlock : StateObj
{
public:
    Intf intf_;
    bool enabled_ = true;
    bool forwardRst_ = true;
    bool backwardFin_ = false;
    PcapDeviceWrite* writer_;
    string forwardFinMsg_;
    string backwardFinMsg_;
    int bufSize_ = 32768;
    void block(Packet* packet);
};

