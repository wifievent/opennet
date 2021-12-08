#include "pcapdevicewrite.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWrite
// ----------------------------------------------------------------------------
PcapDeviceWrite::PcapDeviceWrite(){
    RtmEntry* entry = NetInfo::instance().rtm().getBestEntry(std::string("8.8.8.8"));
    if (entry != nullptr) {
        Intf* intf = entry->intf();
        if (intf != nullptr)
            intfName_ = intf->name();
    }
}

PcapDeviceWrite::~PcapDeviceWrite() {
    close();
}

bool PcapDeviceWrite::doOpen() {
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

bool PcapDeviceWrite::doClose() {
    if (pcap_ != nullptr) {
        pcap_close(pcap_);
        pcap_ = nullptr;
    }

    intf_ = nullptr;

    return true;
}

Packet::Result PcapDeviceWrite::write(Buf buf) {
    int i = pcap_sendpacket(pcap_, buf.data_, int(buf.size_));
    if (i != 0) {
        return Packet::Fail;
    }
    return Packet::Ok;
}

Packet::Result PcapDeviceWrite::write(Packet* packet) {
    Packet::Result res;
    if (mtu_ != 0 && packet->ipHdr_ != nullptr && packet->ipHdr_->len() > uint16_t(mtu_) && packet->tcpHdr_ != nullptr)
        res = writeMtuSplit(packet, mtu_, Packet::Eth);
    else
        res = write(packet->buf_);
    return res;
}
#endif

