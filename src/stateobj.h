#pragma once

#include "obj.h"

struct StateObj : Obj {
    enum State {
        Closed, // 0
        Opening, // 1
        Opened, // 2
        Closing // 3
    };

    StateObj() {}
    ~StateObj() override;

    bool active() { return state_ == Opened; }
    bool open();
    bool close();

protected:
    virtual bool doOpen();
    virtual bool doClose();

protected:
    State state_{Closed};
};
