#include "wipforwardtable.h"

WIpForwardTable::WIpForwardTable() {
    ipForwardTable_ = PMIB_IPFORWARDTABLE(malloc(sizeof (MIB_IPFORWARDTABLE)));
    if (ipForwardTable_ == nullptr) {
        GTRACE("Error allocating memory");
        return;
    }

    DWORD dwSize = 0;
    if (GetIpForwardTable(ipForwardTable_, &dwSize, 0) ==
            ERROR_INSUFFICIENT_BUFFER) {
        free(ipForwardTable_);
        ipForwardTable_ = PMIB_IPFORWARDTABLE(malloc(dwSize));
        if (ipForwardTable_ == nullptr) {
            GTRACE("Error allocating memory");
            return;
        }
    }

    DWORD dwRetVal = GetIpForwardTable(ipForwardTable_, &dwSize, 0);
    if (dwRetVal != NO_ERROR) {
        GTRACE("GetIpForwardTable failed");
        free(ipForwardTable_);
        ipForwardTable_ = nullptr;
    }
}

WIpForwardTable::~WIpForwardTable() {
    if (ipForwardTable_ != nullptr) {
        free(ipForwardTable_);
        ipForwardTable_ = nullptr;
    }
}
