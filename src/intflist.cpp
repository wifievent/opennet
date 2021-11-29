#include "intflist.h"

IntfList::IntfList()
{
    pcap_if_t* allDevs;
    char errBuf[PCAP_ERRBUF_SIZE];
    int i = pcap_findalldevs(&allDevs, errBuf); // find all devs
    if (i != 0) { // if error occured
        return;
    }

    //
    // Add all interfaces
    //
    pcap_if_t* dev = allDevs;
    i = 1;
    while (dev != nullptr) {
        Intf intf;

        intf.index_ = i;
        intf.name_ = dev->name;
        intf.desc_ = dev->description != nullptr ? dev->description : intf.name_;
        if (intf.desc_ == "") intf.desc_ = intf.name_;
#ifdef WOS_LINUX
        intf.mac_ = getMac(dev->name);
        for(pcap_addr_t* pa = dev->addresses; pa != nullptr; pa = pa->next) {
            sockaddr* addr = pa->addr;
            sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);
            if(addr != nullptr && addr->sa_family == AF_INET)
                intf.ip_ = ntohl(addr_in->sin_addr.s_addr);

            addr = pa->netmask;
            addr_in = reinterpret_cast<sockaddr_in*>(addr);
            if(addr != nullptr && addr->sa_family == AF_INET) {
                intf.mask_ = ntohl(addr_in->sin_addr.s_addr);
            }
        }
#endif
#ifdef WOS_WIN
        PIP_ADAPTER_INFO adapter = WIpAdapterInfo::instance().findByAdapterName(dev->name);
        if (adapter != nullptr) {
            intf.desc_ = adapter->Description;
            if (adapter->AddressLength == WMac::SIZE)
                intf.mac_ = adapter->Address;
            intf.ip_ = std::string(adapter->IpAddressList.IpAddress.String);
            intf.mask_ = std::string(adapter->IpAddressList.IpMask.String);
            intf.gateway_ = std::string(adapter->GatewayList.IpAddress.String);
        }
#endif
        intf.ip_and_mask_ = intf.ip_ & intf.mask_;
        // gateway_ is initialized later
        push_back(intf);
        dev = dev->next;
        i++;
    }
    pcap_freealldevs(allDevs);
}
#include <net/if.h> // for ifreq
#include <sys/ioctl.h> // for SIOCGIFHWADDR
#include <unistd.h> // for close
static Mac getMac(char* intfName) {
    int s = socket(PF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        return Mac::nullMac();
    }

    struct ifreq buffer;
    memset(&buffer, 0x00, sizeof(buffer));
    strncpy(buffer.ifr_name, intfName, IFNAMSIZ - 1);

    int i = ioctl(s, SIOCGIFHWADDR, &buffer);
    close(s);
    if (i == -1) {
        // qDebug() << "ioctl return -1" << intfName << strerror(errno);
        return Mac::nullMac();
    }

    const u_char* p = const_cast<const u_char*>(reinterpret_cast<u_char*>(buffer.ifr_ifru.ifru_hwaddr.sa_data));
    Mac res(p);
    return res;
}

Intf* IntfList::findByName(std::string name) {//for Linux
    for (Intf& intf: *this) {
        if (intf.name() == name)
            return &intf;
    }
    return nullptr;
}

Intf* IntfList::findByIp(Ip ip) {//for Windows
    for (Intf& intf: *this) {
        if (intf.ip() == ip)
            return &intf;
    }
    return nullptr;
}
