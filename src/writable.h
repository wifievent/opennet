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

#include "packet.h"

// ----------------------------------------------------------------------------
// GWritable
// ----------------------------------------------------------------------------
struct Writable {
    virtual Packet::Result write(Buf buf) = 0;
    virtual Packet::Result write(Packet* packet) = 0;
    Packet::Result writeMtuSplit(Packet* packet, size_t mtu);

    std::vector <uint8_t> tempBuffer_;
};
