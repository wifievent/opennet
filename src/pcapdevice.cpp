#include "pcapdevice.h"
PcapDevice::PcapDevice() {
    RtmEntry* entry = rtm().getBestEntry(string("8.8.8.8"));
    if (entry != nullptr) {
        Intf* intf = entry->intf();
        if (intf != nullptr)
            intfName_ = intf->name();
    }
}

Packet::Result PcapDevice::read(Packet* packet) {
    Packet::Result res = PcapCapture::read(packet);
    return res;
}

bool PcapDevice::doOpen() {
    if (intfName_ == "") {
        return false;
    }

    char errBuf[PCAP_ERRBUF_SIZE];
    pcap_ = pcap_open_live(intfName_.c_str(), 0, 0, 0, errBuf);

    if (pcap_ == nullptr) {
        return false;
    }

    intf_ = NetInfo::instance().intfList().findByName(intfName_);
    if (intf_ == nullptr) {
        return false;
    }

    int dataLink = pcap_datalink(pcap_);
    dlt_ = Packet::intToDlt(dataLink);

    return true;
}

bool PcapDevice::doClose() {
    if (pcap_ != nullptr) {
        pcap_close(pcap_);
        pcap_ = nullptr;
    }

    intf_ = nullptr;

    return true;
}

Packet::Result PcapDevice::write(Packet* packet) {
    Packet::Result res;
    if (mtu_ != 0 && packet->buf_.size_ > sizeof(EthHdr) + mtu_ && packet->tcpHdr_ != nullptr)
        res = writeMtuSplit(packet, mtu_);
    else
        res = write(packet->buf_);
    return res;
}

Packet::Result PcapDevice::write(Buf buf)
{
    int i = pcap_sendpacket(pcap_, buf.data_, int(buf.size_));
    if (i != 0) {
        return Packet::Fail;
    }
    return Packet::Ok;
}

Packet::Result PcapDevice::writeMtuSplit(Packet* packet, size_t mtu) {
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

    ipHdr->tlen_ = ntohs(mtu);
    ipHdr->checksum_ = htons(IpHdr::calcChecksum(ipHdr));

    byte* tcpDataData = tcpData.data_;

    size_t ipTcpHdrSize = (ipHdr->hlen() + tcpHdr->off()) * 4;
    size_t totalTcpDataSize = packet->buf_.size_ - (sizeof(EthHdr) + ipTcpHdrSize);
    while (true) {
        if (ipTcpHdrSize + totalTcpDataSize <= mtu) break;

        packet->buf_.size_ = sizeof(EthHdr) + mtu;
        size_t onceTcpDataSize = mtu - ipTcpHdrSize;
        //qDebug() << "onceTcpDataSize =" << onceTcpDataSize; // gilgil temp 2021.07.10
        tcpHdr->checksum_ = htons(TcpHdr::calcChecksum(ipHdr, tcpHdr));
        write(packet->buf_);
        usleep(1000);

        tcpHdr->seqnum_ = htonl(tcpHdr->seqnum() + onceTcpDataSize); // next seq
        totalTcpDataSize -= onceTcpDataSize;
        memcpy(tcpDataData, tcpDataData + onceTcpDataSize, totalTcpDataSize); // next data
    }
    ipHdr->tlen_ = ntohs(ipTcpHdrSize + totalTcpDataSize);
    ipHdr->checksum_ = htons(IpHdr::calcChecksum(ipHdr));
    tcpHdr->checksum_ = htons(TcpHdr::calcChecksum(ipHdr, tcpHdr));
    packet->buf_.size_ = sizeof(EthHdr) + ipTcpHdrSize + totalTcpDataSize;
    Packet::Result res = write(packet->buf_); // gilgil temp 2021.07.10

    packet->buf_ = backupBuf;
    memcpy(packet->buf_.data_, tempBuffer_.data(), packet->buf_.size_);
    return res;
}
