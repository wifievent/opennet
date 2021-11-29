#include "opennet.h"
#include "ip.h"
#include "intf.h"
struct RtmEntry
{
    Ip dst_{0};
    Ip mask_{0};
    Ip gateway_{0};

    int metric_{0};
    Intf* intf_{nullptr};
    string intfName_;

    Ip dst() const { return dst_; }
    Ip mask() const { return mask_; }
    Ip gateway() const { return gateway_; }
    int metric() const { return metric_; }
    Intf* intf() const { return intf_; }
};
