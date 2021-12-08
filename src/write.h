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

#include "stateobj.h"
#include "writable.h"

// ----------------------------------------------------------------------------
// GWrite
// ----------------------------------------------------------------------------
struct Write : StateObj, Writable {

public:
    ~Write() override {}

    Packet::Dlt dlt() { return dlt_; }

protected:
    Packet::Dlt dlt_{Packet::Null};

public:
    Packet::Result write(Buf buf) override;
    Packet::Result write(Packet* packet) override;

};
