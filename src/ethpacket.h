#pragma once

#include "ippacket.h"

struct EthPacket : IpPacket
{
	EthPacket() : IpPacket() {
		dlt_ = Packet::Eth;
	}

	void parse() override;
};
typedef EthPacket *PEthPacket;
