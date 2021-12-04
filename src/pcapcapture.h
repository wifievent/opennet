#pragma once

#include <pcap.h>
#include "capture.h"

struct PcapCapture : Capture
{
public:
	std::string filter_{""};
	int mtu_{0};

public:
	PcapCapture() : Capture() {}
	~PcapCapture() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	Packet::Result read(Packet* packet) override;
	Packet::Result write(Buf buf) override;
	Packet::Result write(Packet* packet) override;
	Packet::Result relay(Packet* packet) override;

	Packet::Dlt dlt() override { return dlt_; }

protected:
	bool pcapProcessFilter(pcap_if_t* dev);

	pcap_t* pcap_{nullptr};
	Packet::Dlt dlt_{Packet::Null};
};
