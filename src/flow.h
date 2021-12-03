#include "buf.h"
struct Flow {

    Flow() {};
    Flow(Mac mac, Ip ip) : mac_(mac), ip_(ip) {}

    Mac mac_;
    Ip ip_;
    struct timeval lastAccess_;
};
