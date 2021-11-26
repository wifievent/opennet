#ifndef INTF_H
#define INTF_H

#include "opennet.h"
#include "ip.h"
#include "mac.h"
#include "rtmentry.h"
class intf
{
public:
    Ip ip_{0};
    Mac mac_{Mac::nullMac()};
    Ip mask_{0};
    Ip gateway_{0};
    string name_;
    Ip ip_and_mask{0};

    intf(){}
    Ip gateway(){return gateway_;}
    Mac mac(){return mac_;}
    Ip ip(){return ip_;}
    Ip mask(){return mask_;}
    string name() const { return name_; }
    bool isSameLanIp(Ip ip_){return (ip_and_mask) == (ip_ & mask_);}
};

#endif // INTF_H
