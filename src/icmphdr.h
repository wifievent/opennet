#include "opennet.h"
struct IcmpHdr
{
    uint8_t type_;
    uint8_t code_;
    uint16_t checksum_;
    uint8_t unused_[4];
};
typedef IcmpHdr* PIcmpHdr;
