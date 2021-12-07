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

    //Data
    packet->tcpData_.data_ = pbyte(packet->tcpHdr_) + sizeof(TcpHdr);
    memcpy(backward->tcpData_.data_,backwardFinMsg_.c_str(),backwardFinMsg_.size());
    backward->tcpData_.size_ = backwardFinMsg_.size();

    //ethernet
    backward->ethHdr_->smac_ = intf_.mac_;
    backward->ethHdr_->dmac_ = packet->ethHdr_->smac_;

    //ip
    backward->ipHdr_->hlen_ = 0x44;
    backward->ipHdr_->sip_ = packet->ipHdr_->dip_;
    backward->ipHdr_->dip_ = packet->ipHdr_->sip_;
    backward->ipHdr_->tlen_ = packet->ipHdr_->hlen()*4 + packet->tcpHdr_->off()*4 + backwardFinMsg_.size();
    backward->ipHdr_->ttl_ = 128;
    backward->ipHdr_->checksum_ = backward->ipHdr_->calcChecksum(backward->ipHdr_);

    //tcp
    backward->tcpHdr_->sport_ = packet->tcpHdr_->dport_;
    backward->tcpHdr_->dport_ = packet->tcpHdr_->sport_;
    backward->tcpHdr_->seqnum_ = packet->tcpHdr_->acknum_;
    backward->tcpHdr_->acknum_ = packet->tcpHdr_->seqnum_ + backwardFinMsg_.size();
    backward->tcpHdr_->hlen_ = ((packet->tcpHdr_->off()*4 + backwardFinMsg_.size())/4 & 0xF) << 4;
    backward->tcpHdr_->flag_ = uint16_t(0b10011);
    backward->tcpHdr_->checksum_ = backward->tcpHdr_->calcChecksum(backward->ipHdr_,backward->tcpHdr_);

    //buf
    backward->buf_.size_ = sizeof(EthHdr)+ sizeof(IpHdr) + sizeof(TcpHdr) + backwardFinMsg_.size();
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

bool TcpBlock::sendForwardBlockPacket(Packet* packet) {
    Packet* forward = packet;

    //ethernet
    forward->ethHdr_->smac_ = intf_.mac_;
    forward->ethHdr_->dmac_ = packet->ethHdr_->dmac_;

    //ip
    forward->ipHdr_->hlen_ = 0x44;
    forward->ipHdr_->sip_ = packet->ipHdr_->sip_;
    forward->ipHdr_->dip_ = packet->ipHdr_->dip_;
    forward->ipHdr_->tlen_ = packet->ipHdr_->hlen()*4 + packet->tcpHdr_->off()*4;
    forward->ipHdr_->ttl_ = packet->ipHdr_->ttl_;
    forward->ipHdr_->checksum_ = forward->ipHdr_->calcChecksum(forward->ipHdr_);

    //tcp
    forward->tcpHdr_->sport_ = packet->tcpHdr_->sport_;
    forward->tcpHdr_->dport_ = packet->tcpHdr_->dport_;
    forward->tcpHdr_->seqnum_ = packet->tcpHdr_->seqnum_;
    forward->tcpHdr_->acknum_ = packet->tcpHdr_->acknum_;
    forward->tcpHdr_->hlen_ = (sizeof(TcpHdr)/4) << 4;
    forward->tcpHdr_->flag_ = uint16_t(0b10110);
    forward->tcpHdr_->calcChecksum(forward->ipHdr_,forward->tcpHdr_);

    //buf
    forward->buf_.size_ = sizeof(EthHdr) + sizeof(IpHdr) + sizeof(TcpHdr);

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
