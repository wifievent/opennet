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

#include <QObject>
#include <QSharedPointer>
#include "gbase.h"
#include "gerr.h"
#include "prop/gprop.h"

// ----------------------------------------------------------------------------
// GObj
// ----------------------------------------------------------------------------
struct G_EXPORT GObj : QObject, GProp {
	Q_OBJECT

public:
	GObj(QObject* parent = nullptr) : QObject(parent) {}
	~GObj() override {}

	QSharedPointer<GErr> err{nullptr};

protected:
	QStringList methodList(QMetaMethod::MethodType methodType);

public:
	QStringList signalList();
	QStringList slotList();

public:
	static bool connect(QObject* sender, const char* signal, QObject* receiver, const char* slot, Qt::ConnectionType type = Qt::AutoConnection);
	static bool connect(QObject *sender, const QMetaMethod &signal, QObject *receiver, const QMetaMethod &slot, Qt::ConnectionType type = Qt::AutoConnection);
	static bool disconnect(QObject* sender, const char* signal, QObject* receiver, const char* slot);
	static bool disconnect(QObject *sender, const QMetaMethod &signal, QObject *receiver, const QMetaMethod &slot);

public:
	static GObj* createInstance(QString className, QObject* parent);
};

// ----------------------------------------------------------------------------
// GPtr
// ----------------------------------------------------------------------------
template <typename T>
struct GPtr {
	GPtr() { p_ = nullptr; }
	GPtr(T* p) { p_ = p; }
	GPtr(T& t) { p_ = &t; }
	T* data() { return p_; }
	operator T*() { return p_; }
	operator T*() const { return p_; }
	bool operator==(const T& other) { return p_ = other.p_; }
protected:
	T* p_;
};

// ----------------------------------------------------------------------------
// GObjPtr
// ----------------------------------------------------------------------------
struct GObjPtr : GPtr<GObj> {
	GObjPtr() : GPtr(nullptr) {}
	GObjPtr(GObj* obj) : GPtr(obj) {}
	GObjPtr(GObj& obj) : GPtr(obj) {}
};
Q_DECLARE_METATYPE(GObjPtr)

// ----------------------------------------------------------------------------
// GObjRef
// ----------------------------------------------------------------------------
struct GObjRef : GPtr<GObj> {
	GObjRef() : GPtr(nullptr) {}
	GObjRef(GObj* obj) : GPtr(obj) {}
	GObjRef(GObj& obj) : GPtr(obj) {}
};
Q_DECLARE_METATYPE(GObjRef)

// ----------------------------------------------------------------------------
// GObjRefArrayPtr
// ----------------------------------------------------------------------------
struct _GObjRefArray : QList<GObjRef> {
	virtual GObj* addObj() = 0;
	virtual void delObj(GObj* obj) = 0;
};
typedef _GObjRefArray* GObjRefArrayPtr;
Q_DECLARE_METATYPE(GObjRefArrayPtr)

template <typename T>
struct GObjRefArray : _GObjRefArray {
	virtual ~GObjRefArray() {
		clear();
	}
	GObj* addObj() override {
		GObj* obj = new T;
		push_back(obj);
		return obj;
	}
	void delObj(GObj* obj) override {
		int index = indexOf(obj);
		if (index != -1) {
			delete obj;
			removeAt(index);
		}
	}
	void clear() {
		for (GObj* obj: *this)
			delete obj;
		_GObjRefArray::clear();
	}
};

// ----------------------------------------------------------------------------
// GObjList
// ----------------------------------------------------------------------------
struct G_EXPORT GObjList : QObject, QList<GObj*> {
	GObjList(QObject *parent = nullptr) : QObject(parent) {}
	~GObjList() override {}

	void clear();
	void load(QJsonArray ja);
	void save(QJsonArray& ja);
};

// ----------------------------------------------------------------------------
// SET_ERR
// ----------------------------------------------------------------------------
#define SET_ERR(CODE, MSG) { \
	if (err == nullptr) { \
	err = QSharedPointer<GErr>(new GErr((CODE), (MSG))); \
	qWarning() << err; \
	} \
}
