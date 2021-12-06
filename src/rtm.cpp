#include "rtm.h"
//TODO
//
// ip route show table 0 output
//
// [kali linux]
// default via 10.2.2.1 dev eth0 proto dhcp metric 100 (A)
// 10.2.2.0/24 dev eth0 proto kernel scope link src 10.2.2.3 metric 100 (B)
//
// [android]
// default via 10.2.2.1 dev wlan0  table 1021  proto static (C)
// 10.2.2.0/24 dev wlan0  proto kernel  scope link  src 10.2.2.189 (D)
//
Rtm::Rtm() {
    std::string command("ip route show table 0");
    FILE* p = popen(command.data(), "r");
    if (p == nullptr) {
        return;
    }

    while (true) {
        char buf[256];
        if (std::fgets(buf, 256, p) == nullptr) break;
        RtmEntry entry;
        if (checkA(buf, &entry))
            push_back(entry);
        else if (checkB(buf, &entry))
            push_back(entry);
        else if (checkC(buf, &entry))
            push_back(entry);
        else if (checkD(buf, &entry))
            push_back(entry);
    }
    pclose(p);
}

#ifdef WOS_WIN
#include <cassert>
#include "net/_win/wipadapterinfo.h"
#include "net/_win/wipforwardtable.h"

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

#ifdef WOS_MAC
WRtm::WRtm() {	// TODO
}
#endif

RtmEntry* Rtm::getBestEntry(Ip ip) {
    RtmEntry* res = nullptr;

    for (RtmEntry& entry: *this) {
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

Ip Rtm::findGateway(std::string intfName, Ip ip) {
    for (RtmEntry& entry: *this) {
        if (entry.intf() == nullptr) continue;
        if (entry.intf()->name() != intfName) continue;
        if (entry.gateway_ == Ip::nullIp()) continue;
        if (entry.gateway_ == ip) continue;
        return entry.gateway_;
    }
    return Ip(0);
}


bool Rtm::checkA(char* buf, RtmEntry* entry) {
    char gateway[256];
    char intf[256];
    int metric;
    // default via 10.2.2.1 dev eth0 proto dhcp metric 100 (A)
    int res = sscanf(buf, "default via %s dev %s proto dhcp metric %d", gateway, intf, &metric);
    if (res == 3) {
        entry->gateway_ = Ip(gateway);
        entry->intfName_ = intf;
        entry->metric_ = metric;
        return true;
    }
    return false;
}

bool Rtm::checkB(char* buf, RtmEntry* entry) {
    char cidr[256];
    char intf[256];
    char myip[256];
    int metric;
    // 10.2.2.0/24 dev eth0 proto kernel scope link src 10.2.2.3 metric 100 (B)
    int res  = sscanf(buf, "%s dev %s proto kernel scope link src %s metric %d", cidr, intf, myip, &metric);
    if (res == 4) {
        uint32_t dst;
        uint32_t mask;
        if (!decodeCidr(cidr, &dst, &mask)) return false;
        entry->dst_ = dst;
        entry->mask_ = mask;
        entry->intfName_ = intf;
        entry->metric_ = metric;
        return true;
    }
    return false;
}

bool Rtm::checkC(char* buf, RtmEntry* entry) {
    char gateway[256];
    char intf[256];
    // default via 10.2.2.1 dev wlan0  table 1021  proto static (C)
    int res = sscanf(buf, "default via %s dev %s", gateway, intf);
    if (res == 2) {
        entry->gateway_ = Ip(gateway);
        entry->intfName_ = intf;
        return true;
    }
    return false;
}

bool Rtm::checkD(char* buf, RtmEntry* entry) {
    char cidr[256];
    char intf[256];
    char ip[256];
    // 10.2.2.0/24 dev wlan0  proto kernel  scope link  src 10.2.2.189 (D)
    int res = sscanf(buf, "%s dev %s proto kernel scope link src %s", cidr, intf, ip);
    if (res == 3) {
        uint32_t dst;
        uint32_t mask;
        if (!decodeCidr(cidr, &dst, &mask)) return false;
        entry->dst_ = dst;
        entry->mask_ = mask;
        entry->intfName_ = intf;
        return true;
    }
    return false;
}

bool Rtm::decodeCidr(std::string cidr, uint32_t* dst, uint32_t* mask) {
    size_t found = cidr.find("/");
    if (found == std::string::npos) return false;
    std::string dstStr = cidr.substr(0, found);
    *dst = Ip(dstStr.data());
    std::string maskStr = cidr.substr(found + 1);
    *mask = numberToMask(std::stoi(maskStr.data()));
    return true;
}

uint32_t Rtm::numberToMask(int number) {
    uint32_t res = 0;
    for (int i = 0; i < number; i++) {
        res = (res >> 1);
        res |= 0x80000000;
    }
    return res;
}

