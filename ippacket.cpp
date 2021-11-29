#include "ippacket.h"

void IpPacket::parse() {
	gbyte* p = buf_.data_;
	uint8_t proto;
	switch (*p & 0xF0) {  // version field
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
		case IpHdr::Tcp: // Tcp
			tcpHdr_ = PTcpHdr(p);
			// p += tcpHdr_->off() * 4;
			tcpData_ = TcpHdr::parseData(ipHdr_, tcpHdr_);
			break;
		case IpHdr::Udp: // Udp
			udpHdr_ = PUdpHdr(p);
			// p += sizeof(GUdpHdr);
			udpData_ = UdpHdr::parseData(udpHdr_);
			break;
		default:
			// qDebug() << "unknown protocol" << proto; // gilgil temp 2019.08.19
			break;
	}
}
