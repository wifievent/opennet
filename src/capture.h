#pragma once
#include "stateobj.h"
#include "ethpacket.h"
struct Capture : StateObj {
public:
    Capture() {};
	~Capture() override;
    static Capture& getInstance() {
        static Capture capture;
        return capture;
    }
bool autoParse_{true};

protected:
	bool autoRead_{true};


public:
    virtual Packet::Result read(Packet* packet);
    virtual Packet::Result write(Buf buf);
    virtual Packet::Result write(Packet* packet);
    virtual Packet::Result relay(Packet* packet);
	virtual Packet::Result drop(Packet* packet);
	virtual Packet::Dlt dlt() { return Packet::Null; }
    Packet::Result writeMtuSplit(Packet* packet, size_t mtu);

// required check
/*protected:
	struct Thread : GThread {
		Thread(GCapture* capture) : GThread(capture) {}
	protected:
		void run() override {
			GCapture* capture = dynamic_cast<GCapture*>(parent());
			Q_ASSERT(capture != nullptr);
			capture->run();
		}
	} thread_{this};*/

	virtual void run();

// signals:
// 	void captured(GPacket* packet);
};
