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

#include <QComboBox>
#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemComboBox
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemComboBox : public GPropItem {
	Q_OBJECT

public:
	GPropItemComboBox(GPropItemParam* param);
	GPropItemComboBox(GPropItemParam* param, QStringList& items);

public:
	QComboBox* comboBox_{nullptr};
};

#endif // QT_GUI_LIB
