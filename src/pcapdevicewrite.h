// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "pcapwrite.h"
#include "netinfo.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWrite
// ----------------------------------------------------------------------------
struct PcapDeviceWrite : PcapWrite {

public:
    std::string intfName_{""};
    int mtu_{1500};

public:
    ~PcapDeviceWrite() override;

protected:
    bool doOpen() override;
    bool doClose() override;

public:
    Intf* intf() { return intf_; }

protected:
    Intf* intf_{nullptr};

public:
    Packet::Result write(Buf buf) override;

    Packet::Result write(Packet* packet) override;


};
