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

// ----------------------------------------------------------------------------
// GLogStdout
// ----------------------------------------------------------------------------
struct G_EXPORT GLogStdout : GLog {
	Q_OBJECT

public:
	Q_INVOKABLE GLogStdout(QObject* parent = nullptr) : GLog(parent) {}
	~GLogStdout() override {}

	void write(QString& msg) override;
};
