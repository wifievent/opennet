#include "buf.h"
struct Flow {

    Flow() {};
    Flow(Mac mac, Ip ip) : mac_(mac), ip_(ip) {}

    Mac mac_;
    Ip ip_;
    struct timeval lastAccess_;

    bool operator < (const Flow& flow) const {
        if (this->ip_ < flow.ip_) return true;
        if (this->ip_ > flow.ip_) return false;
        return false;
    }
};
