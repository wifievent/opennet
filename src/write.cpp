#include "write.h"

// ----------------------------------------------------------------------------
// GWrite
// ----------------------------------------------------------------------------
Packet::Result Write::write(Buf buf) {
    (void)buf;
    return Packet::Fail;
}

Packet::Result Write::write(Packet* packet) {
    (void)packet;
    return Packet::Fail;
}
