#include "capture.h"

Capture::~Capture() {
	close();
}

// required check
/*bool GCapture::doOpen() {
	if (!enabled_) return true;

	if (autoRead_) {
		thread_.start();
	}
	return true;
}

bool GCapture::doClose() {
	if (!enabled_) return true;

	bool res = true;
	if (autoRead_) {
		thread_.quit();
		res = thread_.wait();
	}
	return res;
}*/

Packet::Result Capture::read(Packet* packet) {
  (void)packet;
  // virtual function call
	std::cout << "virtual function call" << std::endl;
	return Packet::Fail;
}

Packet::Result Capture::write(Buf buf) {
	(void)buf;
	// virtual function call
	std::cout << "virtual function call" << std::endl;
	return Packet::Fail;
}

Packet::Result Capture::write(Packet* packet) {
	(void)packet;
	// virtual function call
	std::cout << "virtual function call" << std::endl;
	return Packet::Fail;
}

Packet::Result Capture::relay(Packet* packet) {
	(void)packet;
	// virtual function call
	std::cout << "virtual function call" << std::endl;
	return Packet::Fail;
}

Packet::Result Capture::drop(Packet* packet) {
	(void)packet;
  // virtual function call
	std::cout << "virtual function call" << std::endl;
	return Packet::Ok;
}

void Capture::run() {
	EthPacket ethPacket;
	IpPacket ipPacket;

	Packet* packet;
	switch(dlt()) {
		case Packet::Eth:
      packet = &ethPacket;
      break;
		case Packet::Ip:
      packet = &ipPacket;
      break;
	}

	PathType pt = pathType();
	while (active()) {
		Packet::Result res = read(packet);
		if (res == Packet::None) continue;
		if (res == Packet::Eof || res == Packet::Fail) break;
		// emit captured(packet);
		
    // required check
    /*if (pt == InPath) {
			if (packet->ctrl.block_)
				res = drop(packet);
			else
				res = relay(packet);
			if (res != Packet::Ok) {
        // warning
        std::cout << "relay return " << res << std::endl;
			}*/
    }
}
	// emit closed();
