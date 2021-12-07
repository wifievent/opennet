#include "tcpblock.h"
bool TcpBlock::doOpen() {
    if (writer_ == nullptr) {
        return false;
    }
    return true;
}

bool TcpBlock::doClose() {

    return true;
}

bool TcpBlock::sendBackwardBlockPacket(Packet* packet) {
    Packet* backward = packet;
    TcpHdr* tcpHdr = backward->tcpHdr_;
    backward->tcpData_.data_ = pbyte(tcpHdr) + sizeof(TcpHdr);
    memcpy(backward->tcpData_.data_, backwardFinMsg_.c_str(), backwardFinMsg_.size());
    backward->tcpData_.size_ = backwardFinMsg_.size();
    std::swap(tcpHdr->sport_, tcpHdr->dport_);
    tcpHdr->seq_ = htonl(tcpHdr->ack());
    tcpHdr->ack_ = htonl(tcpHdr->seq() + packet->tcpData_.size_ + ((packet->tcpHdr_->flags() & TcpHdr::Ack) != 0 || (packet->tcpHdr_->flags() & TcpHdr::Fin) != 0 ? 1 : 0));
    tcpHdr->flags_ = TcpHdr::Fin | TcpHdr::Ack | TcpHdr::Psh;
    tcpHdr->off_rsvd_ = (sizeof(TcpHdr) / 4) << 4;
    tcpHdr->flags_ &= ~TcpHdr::Syn;

    IpHdr* ipHdr = backward->ipHdr_;
    ipHdr->len_ = htons(sizeof(IpHdr) + sizeof(TcpHdr) + backwardFinMsg_.size());
    ipHdr->tos_ = 0x44;
    ipHdr->ttl_ = 0x80;
    std::swap(ipHdr->sip_, ipHdr->dip_);

    tcpHdr->sum_ = htons(TcpHdr::calcChecksum(ipHdr, tcpHdr));
    ipHdr->sum_ = htons(IpHdr::calcChecksum(ipHdr));

    PcapDevice* pcapDevice = dynamic_cast<PcapDevice*>(writer_);
    if (pcapDevice != nullptr && packet->ethHdr_ != nullptr) {
        EthHdr* ethHdr = backward->ethHdr_;
        Mac myMac = pcapDevice->intf()->mac();
        ethHdr->dmac_ = ethHdr->smac();
        ethHdr->smac_ = myMac;
    }

    // buf size
    size_t bufSize = 0;
    bufSize += sizeof(IpHdr) + sizeof(TcpHdr);
    bufSize += backwardFinMsg_.size();
    backward->buf_.size_ = bufSize;


    Packet::Result res;
    for (int i = 0 ; i < 3 ; i++) {
        res = writer_->write(backward);
        std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
        if (res != Packet::Ok) {
            spdlog::info("Tcpblock::backward::error");
            return false;
        }
    }
    spdlog::info("Tcpblock::backward::success");
    return true;
}

bool TcpBlock::sendForwardBlockPacket(Packet* packet)
{
    Packet* forward = packet;
    TcpHdr* tcpHdr = forward->tcpHdr_;
    tcpHdr->ack_ = htonl(tcpHdr->ack());
    tcpHdr->seq_ = htonl(tcpHdr->seq() + packet->tcpData_.size_ + ((packet->tcpHdr_->flags() & TcpHdr::Ack) != 0 || (packet->tcpHdr_->flags() & TcpHdr::Fin) != 0 ? 1 : 0));
    tcpHdr->flags_ = TcpHdr::Rst | TcpHdr::Ack;
    tcpHdr->win_ = 0;
    tcpHdr->off_rsvd_ = (sizeof(TcpHdr) / 4) << 4;
    tcpHdr->flags_ &= ~TcpHdr::Syn;

    IpHdr* ipHdr = forward->ipHdr_;
    ipHdr->len_ = htons(sizeof(IpHdr) + sizeof(TcpHdr));
    ipHdr->tos_ = 0x44;

    tcpHdr->sum_ = htons(TcpHdr::calcChecksum(ipHdr, tcpHdr));
    ipHdr->sum_ = htons(IpHdr::calcChecksum(ipHdr));

    PcapDevice* pcapDevice = dynamic_cast<PcapDevice*>(writer_);
    if (pcapDevice != nullptr && packet->ethHdr_ != nullptr) {
        EthHdr* ethHdr = forward->ethHdr_;
        Mac myMac = pcapDevice->intf()->mac();
        ethHdr->smac_ = myMac;
    }

    size_t bufSize = 0;
    bufSize += sizeof(IpHdr) + sizeof(TcpHdr);
    packet->buf_.size_ = bufSize;


    Packet::Result res;
    for (int i = 0 ; i < 3 ; i++) {
        res = writer_->write(forward);
        std::this_thread::sleep_for(std::chrono::milliseconds(sendSleepTime_));
        if (res != Packet::Ok) {
            spdlog::info("Tcpblock::forward::error");
            return false;
        }
    }
    spdlog::info("Tcpblock::forward::success");
    return true;
}

void TcpBlock::block(Packet* packet) {
    if (!enabled_) return;
    if (packet->tcpHdr_ == nullptr) return;

    Packet::Dlt dlt = packet->dlt();
    if (dlt != Packet::Eth && dlt != Packet::Ip) {
        return;
    }

    if (forwardRst_) {
        sendForwardBlockPacket(packet);
    }
    if (backwardFin_) {
        sendBackwardBlockPacket(packet);
    }
    packet->ctrl.block_ = true;
}
