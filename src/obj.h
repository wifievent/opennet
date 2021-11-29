#pragma once

#include <iostream>

class Obj {
public:
	typedef enum {
		Closed, // 0
    Closing, // 1
		Opening, // 2
		Opened, // 3
	} State;

  Obj();
  ~Obj();

	bool isActive();

	virtual bool open();
	virtual bool close();

// signal (must change)
// signals:
// 	void opened();
// 	void closed();

protected:
	virtual bool doOpen();
	virtual bool doClose();

protected:
	State state{Closed};
};
