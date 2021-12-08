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

#include "write.h"

// ----------------------------------------------------------------------------
// GPcapWrite
// ----------------------------------------------------------------------------
struct PcapWrite : Write {

public:
    ~PcapWrite() override {}

protected:
    pcap_t* pcap_{nullptr};
};
