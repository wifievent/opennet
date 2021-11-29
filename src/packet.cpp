#include "packet.h"

void Packet::clear(){
    udpData_.clear();
    tcpData_.clear();
    buf_.clear();
    ethHdr_ = nullptr;
    arpHdr_ = nullptr;
    ipHdr_ = nullptr;
    udpHdr_ = nullptr;
    tcpHdr_ = nullptr;
    icmpHdr_ = nullptr;
}

void Packet::parse() {
    ethHdr_ = PEthHdr(buf_.data_);
    switch (ethHdr_->type()) {
        case EthHdr::Ip4:
        case EthHdr::Ip6: {
            byte* p = buf_.data_ + sizeof(EthHdr);
            uint8_t proto;
            switch (*p & 0xF0) {
                case 0x40: // version 4
                    ipHdr_ = PIpHdr(p);
                    proto = ipHdr_->p();
                    p += ipHdr_->hl() * 4;
                    break;
                default:
                    proto = 0; // unknown
                    break;
            }

            switch (proto) {
                case IpHdr::TCP: // Tcp
                    tcpHdr_ = PTcpHdr(p);
                    // p += tcpHdr_->off() * 4;
                    tcpData_ = TcpHdr::parseData(ipHdr_, tcpHdr_);
                    break;
                case IpHdr::UDP: // Udp
                    udpHdr_ = PUdpHdr(p);
                    // p += sizeof(GUdpHdr);
                    udpData_ = UdpHdr::parseData(udpHdr_);
                    break;
                case IpHdr::ICMP: // Icmp
                    icmpHdr_ = PIcmpHdr(p);
                    // p += sizeof(GIcmpHdr);
                    break;
                default:
                    break;
            }
            break;
        }
        case EthHdr::Arp:
            arpHdr_ = PArpHdr(buf_.data_ + sizeof(EthHdr));
            break;
    }
}

Packet::Dlt Packet::intToDlt(int dataLink) {
    Dlt res;
    switch (dataLink) {
        case DLT_EN10MB:
            res = Packet::Eth;
            break;
        case DLT_RAW:
        case DLT_IPV4:
        case DLT_IPV6:
            res = Packet::Ip;
            break;
        case DLT_IEEE802_11_RADIO:
            res = Packet::Dot11;
            break;
        case DLT_NULL:
        default:
            res = Packet::Null;
            break;
    }
    return res;
}

void Packet::copyFrom(Packet* source, Buf newBuf) {
    clear();
    ts_ = source->ts_;
    buf_ = newBuf;
    ctrl = source->ctrl;
    parse();
}
