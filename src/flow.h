#include "buf.h"
struct Flow {

    Flow() {};
    Flow(Mac mac, Ip ip) : mac_(mac), ip_(ip) {}

    Mac mac_{Mac::nullMac()};
    Ip ip_{0};
    struct timeval lastAccess_{0,0};

    bool operator < (const Flow& flow) const {
        if (this->ip_ < flow.ip_) return true;
        if (this->ip_ > flow.ip_) return false;
        return false;
    }
};
