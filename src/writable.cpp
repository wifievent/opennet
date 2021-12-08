#include "writable.h"

// ----------------------------------------------------------------------------
// GWritable
// ----------------------------------------------------------------------------

Packet::Result Writable::writeMtuSplit(Packet* packet, size_t mtu) {
    //qDebug() << packet->buf_.size_; // gilgil temp 2021.07.10

    EthHdr* ethHdr = packet->ethHdr_;
    if (ethHdr == nullptr) {
        return Packet::Fail;
    }

    IpHdr* ipHdr = packet->ipHdr_;
    if (ipHdr == nullptr) {
        return Packet::Fail;
    }

    TcpHdr* tcpHdr = packet->tcpHdr_;
    if (tcpHdr == nullptr) {
        return Packet::Fail;
    }

    Buf tcpData = packet->tcpData_;
    if (tcpData.data_ == nullptr) {
        return Packet::Fail;
    }

    Buf backupBuf = packet->buf_;
    tempBuffer_.resize(packet->buf_.size_);
    memcpy(tempBuffer_.data(), packet->buf_.data_, packet->buf_.size_);

    ipHdr->len_ = ntohs(mtu);
    ipHdr->sum_ = htons(IpHdr::calcChecksum(ipHdr));

    byte* tcpDataData = tcpData.data_;

    size_t ipTcpHdrSize = (ipHdr->hl() + tcpHdr->off()) * 4;
    size_t totalTcpDataSize = packet->buf_.size_ - (sizeof(EthHdr) + ipTcpHdrSize);
    while (true) {
        if (ipTcpHdrSize + totalTcpDataSize <= mtu) break;

        packet->buf_.size_ = sizeof(EthHdr) + mtu;
        size_t onceTcpDataSize = mtu - ipTcpHdrSize;
        //qDebug() << "onceTcpDataSize =" << onceTcpDataSize; // gilgil temp 2021.07.10
        tcpHdr->sum_ = htons(TcpHdr::calcChecksum(ipHdr, tcpHdr));
        write(packet->buf_);
        usleep(1000);

        tcpHdr->seq_ = htonl(tcpHdr->seq() + onceTcpDataSize); // next seq
        totalTcpDataSize -= onceTcpDataSize;
        memcpy(tcpDataData, tcpDataData + onceTcpDataSize, totalTcpDataSize); // next data
    }
    ipHdr->len_ = ntohs(ipTcpHdrSize + totalTcpDataSize);
    ipHdr->sum_ = htons(IpHdr::calcChecksum(ipHdr));
    tcpHdr->sum_ = htons(TcpHdr::calcChecksum(ipHdr, tcpHdr));
    packet->buf_.size_ = sizeof(EthHdr) + ipTcpHdrSize + totalTcpDataSize;
    Packet::Result res = write(packet->buf_); // gilgil temp 2021.07.10

    packet->buf_ = backupBuf;
    memcpy(packet->buf_.data_, tempBuffer_.data(), packet->buf_.size_);
    return res;
}
