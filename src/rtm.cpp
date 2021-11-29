#include "rtm.h"

#ifdef WOS_LINUX
Rtm::Rtm() {
    std::string command("route -n | awk '{print $1, $2, $3, $5, $8}'","r");
    FILE* p = popen(command.data(), "r");
    if (p == nullptr) {
        return;
    }

    char buf[256];
    char* token;
    int j = 0;

    while(std::fgets(buf, 256, p)){
        RtmEntry entry;
        if(j++ < 2) continue;
        token = strtok(buf," ");
        entry.dst_ = Ip(token);

        token = strtok(buf," ");
        entry.gateway_ = Ip(token);

        token = strtok(buf," ");
        entry.mask_ = Ip(token);

        token = strtok(buf," ");
        entry.metric_ = atoi(token);

        token = strtok(buf," ");
        entry.intfName_ = token;
        push_back(entry);
    }
    pclose(p);
}
#endif
#ifdef WOS_WIN
#include "win/wipadapterinfo.h"
#include "win/wipforwardtable.h"

WRtm::WRtm() {
    PMIB_IPFORWARDTABLE table = WIpForwardTable::instance().ipForwardTable_;
    for (int i = 0; i < int(table->dwNumEntries); i++) {
        PMIB_IPFORWARDROW row = &table->table[i];
        WRtmEntry entry;
        IF_INDEX ifIndex = row->dwForwardIfIndex;
        PIP_ADAPTER_INFO adapter = WIpAdapterInfo::instance().findByComboIndex(ifIndex);
        if (adapter == nullptr) continue;
        std::string adapterName = adapter->AdapterName;
        assert(adapterName != "");
        entry.intfName_ = adapterName;
        entry.dst_ = ntohl(row->dwForwardDest);
        entry.gateway_ = ntohl(row->dwForwardNextHop);
        entry.mask_ = ntohl(row->dwForwardMask);
        entry.metric_ = int(row->dwForwardMetric1);
        // intf_ is initialized later
        push_back(entry);
    }
}

#endif
RtmEntry* Rtm::getBestEntry(Ip ip) {
    RtmEntry* res = nullptr;

    for (auto iter = begin(); iter!=end(); iter++) {
        RtmEntry& entry = const_cast<RtmEntry&>(*iter);

        if ((entry.dst_ & entry.mask_) != (ip & entry.mask_)) continue;
        if (res == nullptr) {
            res = &entry;
            continue;
        }
        if (entry.mask_ > res->mask_) {
            res = &entry;
            continue;
        } else
        if (entry.mask_ == res->mask_) {
            if (entry.metric_ < res->metric_) {
                res = &entry;
                continue;
            }
        }
    }

    return res;
}

Ip Rtm::findGateway(string intfName, Ip ip) {
    for (RtmEntry& entry: *this) {
        if (entry.intf() == nullptr) continue;
        if (entry.intf()->name() != intfName) continue;
        if (entry.gateway_ == 0) continue;
        if (entry.gateway_ == ip) continue;
        return entry.gateway_;
    }
    return Ip(0);
}
