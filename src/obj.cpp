#include "obj.h"

Obj::~Obj() {
  if (state_ != Closed) {
    // critical error
    std::cout << "State is " << state_ << std::endl;
  }
  if (tempBuf_.data_ != nullptr) {
      delete[] tempBuf_.data_ ;
  }
  tempBuf_.clear();
}

bool Obj::active() {
    return state_ == Opened;
}

bool Obj::open() {
    if (state_ != Closed) {
    // not closed
    std::cout << "State is " << state_ << std::endl;
		return false;
	}

    state_ = Opening;
	bool res = doOpen();
	if (!res) {
        state_ = Closing;
		doClose();
        state_ = Closed;
		return false;
	}

    state_ = Opened;
	// emit opened();
	return true;
}

bool Obj::close() {
    if (state_ == Closed)
		return true;

	if (!active()) {
    // not opened
        std::cout << "State is " << state_ << std::endl;
		return false;
	}

    state_ = Closing;
	doClose();
    state_ = Closed;
	// emit closed();
	return true;
}

bool Obj::doOpen() {
  // virtual function call
  std::cout << "virtual function call" << std::endl;
	return false;
}

bool Obj::doClose() {
  // virtual function call
	std::cout << "virtual function call" << std::endl;
	return false;
}
