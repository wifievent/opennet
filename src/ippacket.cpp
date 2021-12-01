#include "ippacket.h"

void IpPacket::parse() {
    byte* p = buf_.data_;
	uint8_t proto;
	switch (*p & 0xF0) {  // version field
		case 0x40: // version 4
			ipHdr_ = PIpHdr(p);
			proto = ipHdr_->p();
            p += ipHdr_->hlen() * 4;
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
	}
}
