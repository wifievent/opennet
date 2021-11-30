#include "packet.h"

struct IpPacket : Packet {
	IpPacket() : Packet() {
		dlt_ = Packet::Ip;
	}

	void parse() override;
};
typedef IpPacket *PIpPacket;