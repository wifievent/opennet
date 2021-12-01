#include "buf.h"
struct Flow {
    Flow() {};
    Flow(Mac mac, Ip ip) : mac_(mac), ip_(ip) {}
    Flow(Mac mac, Ip ip, std::string hostName): mac_(mac), ip_(ip), hostName_(hostName) {}

    Mac mac_;
    Ip ip_;
    std::string hostName_;
    std::string nickName_;
    std::string defaultName();
};
