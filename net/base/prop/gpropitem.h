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

#ifdef QT_GUI_LIB

#include <QDebug> // gilgil temp 2016.10.21
#include <QMetaProperty>
#include <QJsonObject>
#include <QTreeWidgetItem>

#include "base/gbase.h"

// ----------------------------------------------------------------------------
// GPropItemParam
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemParam {
	GPropItemParam(QTreeWidget* treeWidget, QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
		: treeWidget_(treeWidget), parent_(parent), object_(object), mpro_(mpro) {}

	QTreeWidget* treeWidget_;
	QTreeWidgetItem* parent_;
	QObject* object_;
	QMetaProperty mpro_;
};

// ----------------------------------------------------------------------------
// GPropItem
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItem : public QObject {
	GPropItem(GPropItemParam* param);
	virtual void update() = 0;

	QTreeWidgetItem* item_{nullptr};
	QObject* object_{nullptr};
	QMetaProperty mpro_;
};

#endif // QT_GUI_LIB
