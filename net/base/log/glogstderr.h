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
// GLogStderr
// ----------------------------------------------------------------------------
struct G_EXPORT GLogStderr : GLog {
	Q_OBJECT

public:
	Q_INVOKABLE GLogStderr(QObject* parent = nullptr) : GLog(parent) {}
	~GLogStderr() override {}

	void write(QString& msg) override;
};
