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

#include <QLineEdit>
#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemLineEdit
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemLineEdit : public GPropItem {
	GPropItemLineEdit(GPropItemParam* param);

public:
	QLineEdit* lineEdit_{nullptr};
};

#endif // QT_GUI_LIB
