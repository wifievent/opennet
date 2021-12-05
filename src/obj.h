#pragma once

#include <iostream>
#include "base.h"
#include "packet.h"
#include "buf.h"
#include "json.h"

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

    virtual void load(Json::Value& json);
    virtual void save(Json::Value& json);
public:
	virtual bool open();
	virtual bool close();

protected:
	virtual bool doOpen();
	virtual bool doClose();

protected:
	State state_{Closed};
};
