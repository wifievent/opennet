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

#include "gpropitem-plaintextedit.h"

// ----------------------------------------------------------------------------
// GPropItemStringList
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemStringList : public GPropItemPlainTextEdit {
	Q_OBJECT

public:
	GPropItemStringList(GPropItemParam* param);
	void update() override;

protected slots:
	void myEditingFinished();
};

#endif // QT_GUI_LIB
