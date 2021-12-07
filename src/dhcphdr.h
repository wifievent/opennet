#include "buf.h"
#pragma pack(push, 1)
struct DhcpHdr
{
    uint8_t mtype_;
    uint8_t htype_;
    uint8_t hlen_;
    uint8_t hops_;
    uint32_t tid_;
    uint16_t secElap_;
    uint16_t bFlag_;
    Ip clientIp_;
    Ip yourIp_;
    Ip serverIp_;
    Ip gatewayIp_;
    Mac cliMac_;
    uint8_t padding_[10];
    uint8_t add_[192];
    uint16_t cookie_;

    uint8_t type() { return mtype_; }
    uint8_t hrd() { return htype_; }
    uint8_t hln() { return hlen_; }
    uint8_t hops() { return hops_; }
    uint32_t transaction() { return ntohl(tid_); }
    uint16_t elapsed() { return ntohs(secElap_); }
    uint16_t bootp() { return ntohs(bFlag_); }
    Ip clientIp() { return ntohl(clientIp_); }
    Ip yourIp() { return ntohl(yourIp_); }
    Ip serverIp() { return ntohl(serverIp_); }
    Ip relayIp() { return ntohl(gatewayIp_); }
    Mac clientMac() { return cliMac_; }
    uint8_t* padding() { return padding_; }

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
typedef DhcpHdr *PDhcpHdr;
#pragma pack(pop)
