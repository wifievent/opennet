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

#include <errno.h>
#include <QDebug>
#include <QSharedPointer>
#include <QString>
#include "gerrcategory.h"
#include "gbase.h"

// ----------------------------------------------------------------------------
// GErr
// ----------------------------------------------------------------------------
struct G_EXPORT GErr {
	enum {
		OK = 0,
		FAIL = -1,
		UNKNOWN = GErrCategory::BASE,
		NOT_SUPPORTED,
		NOT_OPENED_STATE,
		NOT_CLOSED_STATE,
		VIRTUAL_FUNCTION_CALL,
		UNREACHABLE_CODE,
		VALUE_IS_NULL,
		VALUE_IS_NOT_NULL,
		VALUE_IS_ZERO,
		VALUE_IS_NOT_ZERO,
		OBJECT_IS_NULL,
		OBJECT_IS_NOT_NULL,
		RETURN_NULL,
		RETURN_NOT_NULL,
		FILE_NAME_NOT_SPECIFIED,
		INTERFACE_NAME_NOT_SPECIFIED,
		CONNECT_FAILED,
		DISCONNECT_FAILED,
		READ_FAILED,
		WRITE_FAILED
	};

	GErr() : code_(OK) {}
	GErr(int code) : code_(code) {}
	GErr(int code, QString msg) : code_(code), msg_(msg) {}
	virtual ~GErr() {}

	virtual const char* name() { return "Err"; }
	virtual int code() { return code_; }
	virtual QString msg() { return msg_; }

protected:
	int code_;
	QString msg_;
};

QDebug G_EXPORT operator << (QDebug os, GErr& err);
QDebug G_EXPORT operator << (QDebug os, GErr&& err);
QDebug G_EXPORT operator << (QDebug os, GErr* err);
QDebug G_EXPORT operator << (QDebug os, QSharedPointer<GErr> err);

// ----------------------------------------------------------------------------
// GLastErr
// ----------------------------------------------------------------------------
struct G_EXPORT GLastErr : public GErr {
	GLastErr() {
		code_ = errno;
		msg_ = strerror(code_);
	}

	const char* name() override { return "LastErr"; }
};
