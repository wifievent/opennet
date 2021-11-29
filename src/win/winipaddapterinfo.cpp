#include "winipaddapterinfo.h"

winIpAddapterInfo::winIpAddapterInfo() {
    pAdapterInfo_ = PIP_ADAPTER_INFO(malloc(sizeof (IP_ADAPTER_INFO)));
    if (pAdapterInfo_ == nullptr) {
        GTRACE("Error allocating memory needed to call GetAdaptersinfo");
        return;
    }

    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    if (GetAdaptersInfo(pAdapterInfo_, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo_);
        pAdapterInfo_ = PIP_ADAPTER_INFO(malloc(ulOutBufLen));
        if (pAdapterInfo_ == nullptr) {
            GTRACE("Error allocating memory needed to call GetAdaptersinfo");
            return;
        }
    }
    DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo_, &ulOutBufLen);
    if (dwRetVal != NO_ERROR) {
        GTRACE("GetAdaptersInfo failed with error: %lu", dwRetVal);
        free(pAdapterInfo_);
        pAdapterInfo_ = nullptr;
    }
}

winIpAddapterInfo::~winIpAddapterInfo() {
    if (pAdapterInfo_ != nullptr) {
        free(pAdapterInfo_);
        pAdapterInfo_ = nullptr;
    }
}

PIP_ADAPTER_INFO winIpAddapterInfo::findByAdapterName(char* value) {
    std::string name = value;
    PIP_ADAPTER_INFO p = pAdapterInfo_;
    while (p != nullptr) {
        std::string adapterName = p->AdapterName;
        if (name.find(adapterName) != std::string::npos)
            return p;
        p = p->Next;
    }
    return nullptr;
}

PIP_ADAPTER_INFO WIpAdapterInfo::findByComboIndex(DWORD comboIndex) {
    PIP_ADAPTER_INFO p = pAdapterInfo_;
    while (p != nullptr) {
        if (comboIndex == p->ComboIndex)
            return p;
        p = p->Next;
    }
    return nullptr;
}
