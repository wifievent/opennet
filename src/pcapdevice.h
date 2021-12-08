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

#include "pcapcapture.h"
#include "netinfo.h"

// ----------------------------------------------------------------------------
// GPcapDevice
// ----------------------------------------------------------------------------
struct PcapDevice : PcapCapture {

public:
    std::string intfName_{""};
    int snapLen_{32768};
    int flags_{1}; // PCAP_OPENFLAG_PROMISCUOUS
    int readTimeout_{-1}; // -1 msec
    int waitTimeout_{1}; // 1 msec

public:
    ~PcapDevice() override;

protected:
    bool doOpen() override;
    bool doClose() override;

public:
    Intf* intf() { return intf_; }

protected:
    Intf* intf_{nullptr};

public:
    Packet::Result read(Packet* packet) override;
};
