#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "../opennet.h"

struct winIpForwardTable {
private: // singleton
    winIpForwardTable();
    virtual ~winIpForwardTable();

protected:
    friend struct Rtm;
    PMIB_IPFORWARDTABLE ipForwardTable_{nullptr};

public:
    static winIpForwardTable& instance() {
        static winIpForwardTable ipForwardTable;
        return ipForwardTable;
    }
};
