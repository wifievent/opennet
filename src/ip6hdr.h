#include "ip6.h"
#pragma pack(push, 1)
struct Ip6Hdr
{
    uint8_t v_pr_;
    uint8_t flags_[3];
    uint16_t en_;
    uint8_t nh_;
    uint8_t hl_;
    Ip6 sip_;
    Ip6 dip_;

    uint8_t v() { return (v_pr_ & 0xF0) >> 4; }
    uint8_t pr() { return v_pr_ & 0x0F; }
    uint8_t* flags() { return flags_; }
    uint8_t nh() { return nh_; }
    uint8_t hl() { return hl_; }
    Ip6 sip() { return sip_; }
    Ip6 dip() { return dip_; }
};
typedef Ip6Hdr *PIp6Hdr;
#pragma pack(pop)
