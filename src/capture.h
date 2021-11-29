#pragma once

#include "packet.h";
#include "obj.h";

class Capture : Obj {
public:
	Capture();
	~Capture();

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	typedef enum {
		InPath,
		OutOfPath
	} PathType;

public:
	Packet::Result read(Packet* packet) override;
	Packet::Result write(Buf buf) override;
	Packet::Result write(Packet* packet) override;
	virtual Packet::Result relay(Packet* packet);
	virtual Packet::Result drop(Packet* packet);

	virtual Packet::Dlt dlt() { return Packet::Null; }
	virtual PathType pathType() { return OutOfPath; }

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
