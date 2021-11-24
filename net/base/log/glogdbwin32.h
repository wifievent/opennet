// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "glog.h"

#ifdef Q_OS_WIN

// ----------------------------------------------------------------------------
// GLogDbWin32
// ----------------------------------------------------------------------------
struct G_EXPORT GLogDbWin32 : GLog {
	Q_OBJECT

public:
	Q_INVOKABLE GLogDbWin32(QObject* parent = nullptr) : GLog(parent) {}
	~GLogDbWin32() override {}

	void write(QString& msg) override;
};

#endif // Q_OS_WIN
