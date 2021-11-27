#include "obj.h"

Obj::~Obj() {
  if (state != Closed) {
    // critical error
    std::cout << "State is " << state << std::endl;
  }
}

bool Obj::isActive() {
	return state == Opened;
}

bool Obj::open() {
	if (state != Closed) {
    // not closed
    std::cout << "State is " << state << std::endl;
		return false;
	}

	state = Opening;
	bool res = doOpen();
	if (!res) {
		state = Closing;
		doClose();
		state = Closed;
		return false;
	}

	state = Opened;
	// emit opened();
	return true;
}

bool Obj::close() {
	if (state == Closed)
		return true;

	if (!isActive()) {
    // not opened
		std::cout << "State is " << state << std::endl;
		return false;
	}

	state = Closing;
	doClose();
	state = Closed;
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
