#pragma once

#include <winsock2.h>
#include <iphlpapi.h>

#include "../opennet.h"

struct winIpAddapterInfo {
private: // singleton
    winIpAddapterInfo();
    virtual ~winIpAddapterInfo();

protected:
    PIP_ADAPTER_INFO pAdapterInfo_{nullptr};

public:
    PIP_ADAPTER_INFO findByAdapterName(char* value);
    PIP_ADAPTER_INFO findByComboIndex(DWORD comboIndex);

    static winIpAddapterInfo& instance() {
        static winIpAddapterInfo ipAdapterInfo;
        return ipAdapterInfo;
    }
};

