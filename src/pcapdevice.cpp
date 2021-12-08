#include "pcapdevice.h"

// ----------------------------------------------------------------------------
// GPcapDevice
// ----------------------------------------------------------------------------
PcapDevice::PcapDevice(){
    RtmEntry* entry = NetInfo::instance().rtm().getBestEntry(std::string("8.8.8.8"));
    if (entry != nullptr) {
        Intf* intf = entry->intf();
        if (intf != nullptr)
            intfName_ = intf->name();
    }
}

PcapDevice::~PcapDevice() {
    close();
}

bool PcapDevice::doOpen() {

    if (intfName_ == "") {
        return false;
    }

    char errBuf[PCAP_ERRBUF_SIZE];
    pcap_ = pcap_open_live(intfName_.c_str(), snapLen_, flags_, readTimeout_, errBuf);
    if (pcap_ == nullptr) {
        return false;
    }


    intf_ = NetInfo::instance().intfList().findByName(intfName_);
    if (intf_ == nullptr) {
        return false;
    }

    return PcapCapture::doOpen();

}

bool PcapDevice::doClose() {
    intf_ = nullptr;

    return PcapCapture::doClose();
}


Packet::Result PcapDevice::read(Packet* packet) {
    Packet::Result res = PcapCapture::read(packet);
    return res;
}
