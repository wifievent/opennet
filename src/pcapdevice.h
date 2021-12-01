#include "packet.h"
#include "obj.h"
#include "rtm.h"
struct PcapDevice : Obj
{
    int mtu_{0};
    string intfName_{""};
    pcap_t* pcap_{nullptr};
    std::vector <gbyte> tempBuffer_;

    Rtm& rtm() { return rtm_; }
    Intf* intf() { return intf_; }
    IntfList* intfList() { return intfList_; }
    Packet::Result write(Buf buf);
    Packet::Result write(Packet* packet);
    Packet::Dlt dlt() { return dlt_; }
    Packet::Result writeMtuSplit(Packet* packet, size_t mtu);
    PcapDevice();
    ~PcapDevice(){close();}

protected:
    Packet::Dlt dlt_{Packet::Null};
    Intf* intf_{nullptr};
    Rtm rtm_;
    IntfList* intfList_{nullptr};
    bool doOpen() override;
    bool doClose() override;
};
