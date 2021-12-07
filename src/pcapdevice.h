#pragma once
#include "pcapcapture.h"
#include "ethpacket.h"
#include "netinfo.h"
struct PcapDevice : PcapCapture
{
    int mtu_{0};
    string intfName_{""};
    std::vector <uint8_t> tempBuffer_;

    Rtm& rtm() { return rtm_; }
    Intf* intf() { return intf_; }
    IntfList* intfList() { return intfList_; }
    Packet::Result write(Buf buf) override;
    Packet::Result write(Packet* packet) override;
    Packet::Dlt dlt() override { return dlt_; }
    Packet::Result writeMtuSplit(Packet* packet, size_t mtu);
    Packet::Result read(Packet* packet) override;
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
