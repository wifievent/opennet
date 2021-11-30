#pragma once
#include "buf.h"
struct Intf
{
    Ip ip_{0};
    Mac mac_{Mac::nullMac()};
    Ip mask_{0};
    Ip gateway_{0};
    string name_;
    Ip ip_and_mask_{0};
    int index_;
    string desc_;

    Intf(){}
    Ip gateway(){return gateway_;}
    Mac mac(){return mac_;}
    Ip ip(){return ip_;}
    Ip mask(){return mask_;}
    string name() const { return name_; }
    bool isSameLanIp(Ip ip_){return (ip_and_mask_) == (ip_ & mask_);}
};
