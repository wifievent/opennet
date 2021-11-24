#ifndef DHCPHDR_H
#define DHCPHDR_H
#include "opennet.h"
#include "ip.h"
#include "mac.h"
class DhcpHdr
{
public:
    uint8_t mtype;
    uint8_t htype;
    uint8_t hlen;
    uint8_t hops;
    uint32_t tid;
    uint16_t secElap;
    uint16_t bFlag;
    Ip clientIp;
    Ip yourIp;
    Ip serverIp;
    Ip gatewayIp;
    mac cliMac;
    uint8_t padding[10];
    uint8_t add[192];
    uint16_t cookie;

    struct Option {
        uint8_t type_;
        uint8_t len_;

        Option* next() {
            Option* res = POption((char*)this + sizeof(type_) + sizeof(len_) + len_);
            if (res->type_ == End) return nullptr;
            return res;
        }

        void* value() { return (char*)this + sizeof(type_) + sizeof(len_); }
    };
    typedef Option *POption;

    Option* first() {
        return POption((char*)this + sizeof(DhcpHdr));
    }

    // OptionType(Option::type_)
    enum: uint8_t {
        HostName = 12,
        RequestedIpAddress = 50,
        End = 255
    };
};

#endif // DHCPHDR_H
