#pragma once

#include <iostream>
#include "base.h"
#include "packet.h"
#include "buf.h"

struct Obj {
public:
	enum State {
		Closed, // 0
        Closing, // 1
		Opening, // 2
		Opened, // 3
	};

    Buf tempBuf_{nullptr, 0};

    Obj();
    virtual ~Obj();
	bool active();

public:
	virtual bool open();
	virtual bool close();
/*
signals:
	void opened();
	void closed();*/

protected:
	virtual bool doOpen();
	virtual bool doClose();

protected:
	State state_{Closed};
};
