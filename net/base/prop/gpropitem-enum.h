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

#include "gpropitem-combobox.h"

// ----------------------------------------------------------------------------
// GPropItemEnum
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemEnum : public GPropItemComboBox {
	Q_OBJECT

public:
	GPropItemEnum(GPropItemParam* param);
	void update() override;

protected slots:
	void myCurrentIndexChanged(int index);
};

#endif // QT_GUI_LIB
