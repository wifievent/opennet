#include "tcpblock.h"

void TcpBlock::block(Packet* packet_){
    if(forwardRst_){
        Packet* forward = packet_;

        //ethernet
        forward->ethHdr_->smac_ = intf_.mac_;
        forward->ethHdr_->dmac_ = packet_->ethHdr_->dmac_;
        //ip
        forward->ipHdr_->sip_ = packet_->ipHdr_->sip_;
        forward->ipHdr_->dip_ = packet_->ipHdr_->dip_;
        forward->ipHdr_->len_ = packet_->ipHdr_->hl()*4 + packet_->tcpHdr_-> off()*4;
        forward->ipHdr_->ttl_ = packet_->ipHdr_->ttl_;
        forward->ipHdr_->checksum_ = forward->ipHdr_->calcChecksum(forward->ipHdr_);
        //tcp
        forward->tcpHdr_->sport_ = packet_->tcpHdr_->sport_;
        forward->tcpHdr_->dport_ = packet_->tcpHdr_->dport_;
        forward->tcpHdr_->seqnum_ = packet_->tcpHdr_->seqnum_;
        forward->tcpHdr_->acknum_ = packet_->tcpHdr_->acknum_;
        forward->tcpHdr_->flag_ = uint16_t(0b10110);
        forward->tcpHdr_->calcChecksum(forward->ipHdr_,forward->tcpHdr_);

    }else if(backwardFin_){
        Packet* backward = packet_;

        //ethernet
        backward->ethHdr_->smac_ = intf_.mac_;
        backward->ethHdr_->dmac_ = packet_->ethHdr_->smac_;
        //ip
        backward->ipHdr_->sip_ = packet_->ipHdr_->dip_;
        backward->ipHdr_->dip_ = packet_->ipHdr_->sip_;
        backward->ipHdr_->len_ = packet_->ipHdr_->hl()*4 + packet_->tcpHdr_-> off()*4 + backwardFinMsg_.size();
        backward->ipHdr_->ttl_ = 128;
        backward->ipHdr_->checksum_ = backward->ipHdr_->calcChecksum(backward->ipHdr_);
        //tcp
        backward->tcpHdr_->sport_ = packet_->tcpHdr_->dport_;
        backward->tcpHdr_->dport_ = packet_->tcpHdr_->sport_;
        backward->tcpHdr_->seqnum_ = packet_->tcpHdr_->acknum_;
        backward->tcpHdr_->acknum_ = packet_->tcpHdr_->seqnum_ + backwardFinMsg_.size();
        backward->tcpHdr_->hlen_ = ((packet_->tcpHdr_->off()*4 + backwardFinMsg_.size())/4 & 0xF) << 4;
        backward->tcpHdr_->flag_ = uint16_t(0b10011);
        backward->tcpHdr_->checksum_ = backward->tcpHdr_->calcChecksum(backward->ipHdr_,backward->tcpHdr_);
    }
}
