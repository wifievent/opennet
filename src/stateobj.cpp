#include "stateobj.h"

StateObj::~StateObj() {
  if (state_ != Closed) {
    // critical error
    spdlog::critical("State is " + state_);
  }
  if (tempBuf_.data_ != nullptr) {
      delete[] tempBuf_.data_ ;
  }
  tempBuf_.clear();
}

bool StateObj::open() {
    if (state_ != Closed) {
        return false;
    }
    err_.clear();

    state_ = Opening;
    bool res = doOpen();
    if (!res) {
        state_ = Closing;
        doClose();
        state_ = Closed;
        return false;
    }

    state_ = Opened;
    return true;
}

bool StateObj::close() {
    if (state_ == Closed)
        return true;

    if (!active()) {
        return false;
    }

    state_ = Closing;
    doClose();
    state_ = Closed;
    return true;
}

bool StateObj::doOpen() {
    return false;
}

bool StateObj::doClose() {
    return false;
}
