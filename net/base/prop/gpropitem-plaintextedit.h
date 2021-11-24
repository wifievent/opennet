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

#include <QPlainTextEdit>
#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemPlainTextEdit_
// ----------------------------------------------------------------------------
struct GPropItemPlainTextEdit;
struct G_EXPORT GPropItemPlainTextEdit_ : QPlainTextEdit {
	Q_OBJECT

public:
	GPropItemPlainTextEdit_(QWidget *parent, GPropItemPlainTextEdit* owner);

signals:
	void editingFinished();

protected:
	void focusOutEvent(QFocusEvent * e) override;

protected:
	GPropItemPlainTextEdit* owner_;
};

// ----------------------------------------------------------------------------
// GPropItemPlainTextEdit
// ----------------------------------------------------------------------------
struct GPropItemPlainTextEdit_;
struct GPropItemPlainTextEdit : public GPropItem {
	GPropItemPlainTextEdit(GPropItemParam* param);

protected:
	GPropItemPlainTextEdit_* plainTextEdit_{nullptr};
};

#endif // QT_GUI_LIB
