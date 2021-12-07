#include "buf.h"
struct Flow {

    Flow() {};
    Flow(Mac tmac, Ip tip,Mac smac, Ip sip) : tmac_(tmac), tip_(tip), smac_(smac), sip_(sip) {}

    Mac tmac_{Mac::nullMac()};
    Ip tip_{0};
    Mac smac_{Mac::nullMac()};
    Ip sip_{0};
    struct timeval lastAccess_{0,0};

    bool operator < (const Flow& flow) const {
        if (this->tip_ < flow.tip_) return true;
        if (this->tip_ > flow.tip_) return false;
        return false;
    }
};
