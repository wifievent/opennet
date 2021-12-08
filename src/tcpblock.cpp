#include "tcpblock.h"
bool TcpBlock::doOpen() {
    if (writer_ == nullptr) {
        return false;
    }
    blockBuf_ = new unsigned char[bufSize_];
    return true;
}

bool TcpBlock::doClose() {
    if (blockBuf_ != nullptr) {
        delete[] blockBuf_;
        blockBuf_ = nullptr;
    }

    return true;
}

void TcpBlock::sendBlockPacket(Packet* packet, TcpBlock::Direction direction, TcpBlock::BlockType blockType, uint32_t seq, uint32_t ack, std::string msg) {
    Packet* blockPacket = nullptr;
    size_t copyLen;

    Packet::Dlt dlt = packet->dlt();
    switch (dlt) {
        case Packet::Eth: blockPacket = &blockEthPacket_; copyLen = sizeof(EthHdr) + sizeof(IpHdr) + sizeof(TcpHdr); break;
        case Packet::Ip: blockPacket = &blockIpPacket_; copyLen = sizeof(IpHdr) + sizeof(TcpHdr); break;
        case Packet::Dot11: blockPacket = nullptr; break;
        case Packet::Null: blockPacket = nullptr; break;
    }
    if (blockPacket == nullptr) {
        return;
    }

    if (int(copyLen) > bufSize_) {
        return;
    }
    memcpy(blockBuf_, packet->buf_.data_, copyLen);
    blockPacket->copyFrom(packet, Buf(blockBuf_, copyLen));

    TcpHdr* tcpHdr = blockPacket->tcpHdr_;

    //
    // Data
    //
    if (blockType == Fin) {
        blockPacket->tcpData_.data_ = pbyte(tcpHdr) + sizeof(TcpHdr);
        memcpy(blockPacket->tcpData_.data_, msg.c_str(), msg.size());
        blockPacket->tcpData_.size_ = msg.size();
    }

    //
    // TCP
    //
    if (direction == Backward)
        std::swap(tcpHdr->sport_, tcpHdr->dport_);
    tcpHdr->seq_ = htonl(seq);
    tcpHdr->ack_ = htonl(ack);
    if (blockType == Rst) {
        tcpHdr->flags_ = TcpHdr::Rst | TcpHdr::Ack;
        tcpHdr->win_ = 0;
    } else {
        tcpHdr->flags_ = TcpHdr::Fin | TcpHdr::Ack | TcpHdr::Psh;
    }
    tcpHdr->off_rsvd_ = (sizeof(TcpHdr) / 4) << 4;
    tcpHdr->flags_ &= ~TcpHdr::Syn;

    //
    // IP
    //
    IpHdr* ipHdr = blockPacket->ipHdr_;
    if (blockType == Rst)
        ipHdr->len_ = htons(sizeof(IpHdr) + sizeof(TcpHdr));
    else
        ipHdr->len_ = htons(sizeof(IpHdr) + sizeof(TcpHdr) + msg.size());
    ipHdr->tos_ = 0x44;
    if (direction == Backward) {
        ipHdr->ttl_ = 0x80;
        std::swap(ipHdr->sip_, ipHdr->dip_);
    }

    //
    // checksum
    //
    tcpHdr->sum_ = htons(TcpHdr::calcChecksum(PIpHdr(ipHdr), tcpHdr));
    ipHdr->sum_ = htons(IpHdr::calcChecksum(PIpHdr(ipHdr)));

    //
    // Ethernet
    //
    PcapDevice* pcapDevice = dynamic_cast<PcapDevice*>(writer_);

    if (pcapDevice->intf()==nullptr){
        spdlog::info("error intf");
    }
    if (pcapDevice != nullptr && packet->ethHdr_ != nullptr) {
        EthHdr* ethHdr = blockPacket->ethHdr_;
        Mac myMac = pcapDevice->intf()->mac();
        if (direction == Backward) {
            ethHdr->dmac_ = ethHdr->smac();
            ethHdr->smac_ = myMac;
        } else {
            //ethHdr->dmac_ = ethHdr->dmac();
            ethHdr->smac_ = myMac;
        }
    }

    // buf size
    size_t bufSize = 0;
    if (dlt == Packet::Eth) bufSize += sizeof(EthHdr);
    bufSize += sizeof(IpHdr) + sizeof(TcpHdr);
    if (blockType == Fin) bufSize += msg.size();
    blockPacket->buf_.size_ = bufSize;

    // write
    writer_->write(blockPacket);
}

void TcpBlock::block(Packet* packet) {
    if (!enabled_) return;
    if (packet->tcpHdr_ == nullptr) return;

    Packet::Dlt dlt = packet->dlt();
    if (dlt != Packet::Eth && dlt != Packet::Ip) {
        return;
    }

    bool synExist = (packet->tcpHdr_->flags() & TcpHdr::Syn) != 0;
    bool rstExist = (packet->tcpHdr_->flags() & TcpHdr::Rst) != 0;
    bool finExist = (packet->tcpHdr_->flags() & TcpHdr::Fin) != 0;
    bool ackExist = (packet->tcpHdr_->flags() & TcpHdr::Ack) != 0;
    if (rstExist || finExist) return;

    TcpHdr* tcpHdr = packet->tcpHdr_;

    size_t tcpDataSize = packet->tcpData_.size_;
    uint32_t seq = tcpHdr->seq();
    uint32_t nextSeq = seq + tcpDataSize + (synExist || finExist ? 1 : 0);
    uint32_t ack = tcpHdr->ack();

    bool _blocked = false;
    if (forwardBlockType_ == Rst) {
        if (synExist && !ackExist)
            sendBlockPacket(packet, Forward, Rst, seq, 0); // useless
        else
            sendBlockPacket(packet, Forward, Rst, nextSeq, ack);
        _blocked = true;
    }
    if (forwardBlockType_ == Fin) {
        if (!synExist) {
            sendBlockPacket(packet, Forward, Fin, nextSeq, ack, forwardFinMsg_);
            _blocked = true;
        }
    }
    if (backwardBlockType_ == Rst) {
        if (synExist && !ackExist)
            sendBlockPacket(packet, Backward, Rst, 0, nextSeq);
        else
            sendBlockPacket(packet, Backward, Rst, ack, nextSeq);
        _blocked = true;
    }
    if (backwardBlockType_ == Fin) {
        if (!synExist) {
            sendBlockPacket(packet, Backward, Fin, ack, nextSeq, backwardFinMsg_);
            _blocked = true;
        }
    }

    if (forwardBlockType_ != None)
        packet->ctrl.block_ = true;
}
